<?php
include_once 'accountFunctions.php';
include_once 'groupFunctions.php';
include_once 'flashCardFunctions.php';
include_once 'whiteboardFunctions.php';
include_once 'utilityFunctions.php';


$server = stream_socket_server("tcp://0.0.0.0:9001", $errno, $errorMessage); //AWS EC2 server
//$server = stream_socket_server("tcp://localhost:1520", $errno, $errorMessage); //Localhost
//echo ++$argv[1];
//$_ = $_SERVER['_'];;
echo "This is the server socket: ";
var_dump($server);
echo "\n+++++++ Program Start +++++++\n";

/* Failed restart function
register_shutdown_function(function () {
  global $_, $argv, $server;
  //restart script
  sleep(5);
  fclose($server);
  pcntl_exec($_, $argv);
});*/

//Returns error message if we fail to bind
if ($server[0] === false)
{
    die("Failed to bind to socket:  $errorMessage \n");
}


//Updates all users to offline on server startup in case of crash.
echo "Resetting all Online Users... \n";
clearAllOnlineStatus();

//Deletes all users in groups just in case of crash.
//Insures that no duplicate users would be printed

echo "Clearing Group Members...\n";
clearGroupMembers();


//Client streaming starts
$clientList = array(); // $ip => ($socket, $username)
while(true) {
    //echo "Listening \n";
    //prepare readable sockets
    echo "\n+++++++++++++++++++++++++\n";
    echo "At top of the while loop, current clients connected: \n";
    echo "Clients:\n";
    var_dump($clientList);

    //$read_socks = array_column($clients, 0);
    $read_socks = getSocketList($clientList);
    /*echo "Sockets we have already read through, obtained by copying client array's resources(aka socket identifier)\n";
    var_dump($read_socks);
    echo "Attach server socket into readable socket\n";*/
    $read_socks["Server Socket: "] = $server;

    echo "Readable sockets:\n";
    var_dump($read_socks);
    echo "+++++++++++++++++++++++++\n \n";

    //start reading and use a large timeout
    if(!stream_select ( $read_socks, $write, $except, 300000 ))
    {
        die('something went wrong while selecting');
    }

    //new client
    if(in_array($server, $read_socks)) //If $server in $read_socks
    {
        $new_client = stream_socket_accept($server);
        if ($new_client) {
            //print remote client information, ip and port number
            echo 'Connection accepted from ' . stream_socket_get_name($new_client, true) . "\n";
            echo "Setting client[ip address]->index 0, to socket resource and index 1 to empty string(reserving for username)\n";
            echo "+++++++++++++++++++++++++++ \n";
            $user = stream_socket_get_name($new_client, true);
            $clientList[$user] = new User();
            $clientList[$user]->setIP(stream_socket_get_name($new_client, true));
            $clientList[$user]->setSocket($new_client);
            echo "DEBUG: Verifying IP is " . $clientList[$user]->getIP() . "\n";
            echo "DEBUG: Verifying socket is " . $clientList[$user]->getSocket() ."\n";
        }
        unset($read_socks[ array_search($server, $read_socks) ]);
    } //close if(in_array)

    //message from existing client
    foreach($read_socks as $sock)
    {   echo "Now iterating through our read sockets to read in the message \n";
        echo "Waiting for data now \n\n";
        $data = fread($sock, 5); //i.e. 01924SVWB$groupID $wbstring
        echo "Number of bytes in message: $data\n";
        if ($data > 0) { //data = 01924
          echo "DATA EXISTS, it will isolate the code and message \n";
          $bytes = (int)$data;//bytes = 1924
          echo "Number of bytes in while loop $bytes \n";
          $messageLength = 0;
          $message = "";
          //$remainingLength = $bytes;
          while(($messageLength < $bytes)){ // 0 < 1924
            echo "In while loop: isolating data \n";
            $newdata = fread($sock, ($bytes-$messageLength)); //read800
            echo "Appending $newdata to $message \n";
            $messageLength = $messageLength + strlen($newdata); //
            echo "messageLength is currently $messageLength out of $bytes total";
            $message = "{$message}{$newdata}";
            //$remainingLength -= strlen($newdata);
          }
          echo "DEBUG: This is the message after reading entire message and isolating \n $bytes $message \n";
        }
        if(!$data)
        {
            echo "DATA DOES NOT EXIST, i.e. client disconnect, it will close their socket\n";

            echo "Here it prints out the array it's going to kill/the one that disconnected\n";
            var_dump(array_search($sock, getSocketList($clientList), true));
            $socketDC = array_search($sock, getSocketList($clientList), true);
            echo "About to delete this IP: $socketDC \n";
            echo "Setting User that just DC'ed to offline \n";
            logout($clientList[$socketDC]);
            echo "Unsetting/deleting the socket that just left from client array then closes it\n";
            /*echo "Should be looking for: \n";
            var_dump($sock);*/
            unset($clientList[ array_search($sock, getSocketList($clientList), true)]);
            @fclose($sock); //closes the socket. @ supresses error messages
            echo "A client disconnected. Now there are total ". count($clientList) . " clients.\n";
            var_dump($clientList);
            continue;
        }
        //send the message back to client
        else {
          echo "It passes the code through our switch statement \n";
          $ip = stream_socket_get_name($sock, true); //Current user.
          $client = $clientList[$ip];

          //Takes in the first 5 bytes as to determine length of message.
          $code = substr($message, 0, 4);
          $msg = substr($message, 4, $bytes);

          //Prints out messages received for debugging purposes.
          if ($code == 'UPWB' || $code == 'SVWB') {
            echo "THIS IS THE MESSAGE BEFORE PASSING INTO SWITCH STATEMENT: {$bytes}{$code}: WB String too long to echo. \n";
            echo "THIS IS THE MESSAGE BEFORE PASSING INTO SWITCH STATEMENT: $code: $msg \n";}
          else if ($code == 'WBLN') {
            //nothing because it would be echo'd too many times because each point is going to be sent over.
          }
          else {
            echo "THIS IS THE MESSAGE BEFORE PASSING INTO SWITCH STATEMENT $code: $msg \n"; }

          $limit = 3;
          if ($code == "GCHT" || $code == "SVWB") {
            $limit = 2;
          }
          $codeMessage = explode(" ", $msg, $limit);  //Puts message into array

          switch($code) {
            case "CACC":
              createAccount($codeMessage[0], $codeMessage[1], $codeMessage[2], $sock);
              break; //email, username, password, socket
            case "LOGN":
              if(loginAccount($codeMessage[0], $codeMessage[1], $sock)) {
                $client->setName($codeMessage[0]);}
              break;//username, password, socket
            case "LOGT":
              logoutAccount($client->getName(), $sock);
              break; //username, socket
            case "CGRP":
              createGroup($codeMessage[0], $client, $sock);
              break; //groupname, user, socket
            case "JGRP":
              if(joinGroup($codeMessage[0], $client, $clientList, $sock)) {
                $client->setGroup($codeMessage[0]);}
              break; //groupID, user, client array, socket
            case "LGRP":
              leaveGroup($codeMessage[0], $client, $clientList, $sock);
              break; //groupID, user, client array, socket
            case "GCHT":
              sendChatMessage($codeMessage[0], $codeMessage[1], $client, $clientList, $sock);
              break; //groupID, message, user, client list, socket
            case "WBLN":
              whiteboardLine($codeMessage[0], $codeMessage[1], $codeMessage[2], $clientList, $sock);
              break; //groupID, point1, point2, client array, socket
            case "UPWB":
              updateWhiteBoard($codeMessage[0], $codeMessage[1], $codeMessage[2], $clientList, $sock);
              break; //ipAddress, wb string, client array, socket
            case "SVWB":
              saveWhiteBoard($codeMessage[0], $codeMessage[1], $sock);
              break; //groupID, wb string, socket
            case "CHPW":
              changePassword($clients[$ip][1], $codeMessage[1], $sock);
              break;
            case "RACC":
              recoverAccount($codeMessage[0], $codeMessage[1], $sock);
              break;
            case "RUSR":
              rememberUsername($codeMessage[0], $sock);
              break;
            case "RUSP":
              rememberPassword($codeMessage[0], $codeMessage[1], $sock);
              break;
            case "FCBK":
              addToCard($codeMessage[0], $codeMessage[1], $codeMessage[2], $client, $clientList, $sock, 2);
              break;
            case "FCFT":
              addToCard($codeMessage[0], $codeMessage[1], $codeMessage[2], $client, $clientList, $sock, 1); // 0 = groupID 1 = card id 2 = message
              break;
          }//Switch Statement
      }//Closes else
    }//Closes foreach
}//Closes while
?>
