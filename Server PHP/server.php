<?php
include_once 'accountFunctions.php';
include_once 'groupFunctions.php';
include_once 'flashCardFunctions.php';
include_once 'whiteboardFunctions.php';
include_once 'utilityFunctions.php';

// $socket_tuple = ($socket, $username)
//$server = stream_socket_server("tcp://0.0.0.0:9001", $errno, $errorMessage); //AWS EC2 server
$server = stream_socket_server("tcp://127.0.0.1:1520", $errno, $errorMessage); //Localhost
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
// Create connection
$connection =  connect();

//Updates all users to offline on server startup.
//connect DB2
$makeoffline = "UPDATE UserInfo
	   SET Status = 'Offline'
	   WHERE Status = 'Online'";
mysqli_query($connection, $makeoffline);
//disconnect DB2
//Deletes all users in groups just in case of crash.
//Insures that no duplicate users would be printerd.
$countGroups = "SELECT TABLE_NAME
              FROM information_schema.columns
              where table_schema = 'StudyGroup'
              and column_name = 'userList';";
$numGroups = getNumRows($connection, $query);
echo "Number of groups in database: $numGroups \n";
clearGroupMembers($connections, $numGroups);
disconnect($connection);


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
    echo "Sockets we have already read through, obtained by copying client array's resources(aka socket identifier)\n";
    var_dump($read_socks);
    echo "Attach server socket into readable socket\n";
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
            echo 'Connection accepted from ' . stream_socket_get_name($new_client, true) . "\n";  //Prints out connection from IP address ^^
            echo "Setting client[ip address]->index 0, to socket resource and index 1 to empty string(reserving for username)\n";
            //Assigns ip address and blank username to the array
            /*$clients[stream_socket_get_name($new_client, true)] = array($new_client, "");*/
            var_dump($clientList);
            echo "+++++++++++++++++++++++++++ \n";
            $user = stream_socket_get_name($new_client, true);
            $clientList[$user] = new User();
            $clientList[$user]->setIP(stream_socket_get_name($new_client, true));
            $clientList[$user]->setSocket($new_client);
            echo "DEBUG: Verifying IP is " . $clientList[$user]->getIP() . "\n";
            echo "DEBUG: Verifying socket is " . $clientList[$user]->getSocket() ."\n";
        }
        //delete the server socket from the read sockets
        echo "DEBUG: Now showing our read sockets and unsetting server.. why?\n";
        var_dump($read_socks);
        unset($read_socks[ array_search($server, $read_socks) ]);
        echo "DEBUG: Now showing what the read sockets look like after removing server from it\n";
        var_dump($read_socks);
        echo "Server's resource id should have disappeared \n \n";
        echo "OH SHIET. It removes the server resource so it doesn't try to read data from that specific resource...? \n";
    } //close if(in_array)

    //message from existing client
    foreach($read_socks as $sock)
    {   echo "Now iterating through our read sockets to read in the message \n";
        // We go through every client socket here.
        // $ip holds the ip address of the client socket
        // $clients[$ip][1] is equal to the username AFTER successful login
        echo "Waiting for data now \n\n";
        $data = fread($sock, 5);
        if ($data > 0) {
          echo "DATA EXISTS, it will isolate the code and message \n";
          $bytes = (int)$data;
          $lennewdata = 0;
          $newestdata = "";
          while(($lennewdata < $bytes)){
            $newdata = fread($sock, $bytes);
            $lennewdata = $lennewdata + strlen($newdata);
            //echo "This is the length of the bytes inside while loop: $lennewdata compared to bytes needed to read: $bytes\n";
            $newestdata = "{$newestdata}{$newdata}";
          }
        }
        if(!$data)
        {
            echo "DATA DOES NOT EXIST, i.e. client disconnect, it will close their socket\n";

            echo "Here it prints out the array it's going to kill/the one that disconnected\n";
            var_dump(array_search($sock, getSocketList($clientList), true));
            //var_dump(array_search($sock, array_column($clients, 0)));  //Array column returns values of index 0 of clients[], which is the socket.  array search looks at the list of sockets array_column returned for the $sock to close. This searches whatever client that disconnected, and returns its index so we know which to delete.

            echo "Unsetting/deleting the socket that just left from client array then closes it\n";
            //unset($clients[ array_search($sock, array_map(function($item){return $item[0];}, $clients)) ]);  //array_map applies the function return $item[0] to all clients.  so I guess that means it's returning the socket for all clients...? then it compares the sockets returned with array search, then the socket that matches array_search gets unset, it deletes it from the array. This searches array(2)?
            unset($clientList[ array_search($sock, getSocketList($clientList), true)]);
            @fclose($sock); //closes the socket. @ supresses error messages
            echo "A client disconnected. Now there are total ". count($clientList) . " clients.\n";
            continue;
            /*
            echo "Hello, how are you doing\n";
            var_dump(array_search($sock, $clients[$user]->getSocket));
            unset($clients[ array_search($sock, array_map(function($item){return $item[0];}, $clients)) ]);
            @fclose($sock);
            echo "Client disconnected. Now there are " . count($clientList) . " clients.\n";
            continue;*/
        }
        //send the message back to client
        else {
          echo "It passes the code through our switch statement \n";
          $ip = $clientList[$user]->getIP();
          //$ip = stream_socket_get_name($sock, true);
          //Takes in the first 5 bytes as to determine length of message.
          $code = substr($newestdata, 0, 4);
          $message = substr($newestdata, 4, $bytes);

          //Prints out messages received for debugging purposes.
          if ($code == 'UPWB' || $code == 'SVWB') {
            echo "THIS IS THE MESSAGE: {$bytes}{$code}: WB String too long to echo. \n"; }
          else if ($code == 'WBLN') {
            //nothing because it would be echo'd too many times because each point is going to be sent over.
          }
          else {
            echo "THIS IS THE MESSAGE $code: $message \n"; }

          $limit = 3;
          if ($code == "GCHT" || $code == "UPWB" || $code == "SVWB") {
            $limit = 2;
          }
          $codeMessage = explode(" ", $message, $limit);  //Puts message into array

          switch($code) {
            case "CACC":
              createAccount($codeMessage[0], $codeMessage[1], $codeMessage[2], $sock);
              break;
            case "LOGN":
              if(loginAccount($codeMessage[0], $codeMessage[1], $sock)) {
                //$clients[$ip][1] = $codeMessage[0]; // Set username to clients dict
                $clients[$user]->setName($codeMessage[0]);
              } break;
            case "LOGT":
              logoutAccount($clientList[$ip][1], $sock);
              break;
            case "CGRP":
              //createGroup($codeMessage[0], $ip, $clients, $sock);
              createGroup($codeMessage[0], $clientList[$user], $sock);
              break;
            case "JGRP":
              joinGroup($codeMessage[0], $ip, $clientList, $sock);
              //joinGroup($codeMessage[0], $clients[$user], $sock);
              break;
            case "LGRP":
              //leaveGroup($codeMessage[0], $ip, $clients, $sock);
              leaveGroup($codeMessage[0], $clientList[$user], $sock);
              break;
            case "GCHT":
              //sendChatMessage($codeMessage[0], $codeMessage[1], $ip, $clients, $sock);
              sendChatMessage($codeMessage[0], $codeMessage[1], $clientList[$user], $sock);
              break;
            case "WBLN":
              //whiteboardLine($codeMessage[0], $codeMessage[1], $codeMessage[2], $ip, $clients, $sock);
              whiteboardLine($codeMessage[0], $codeMessage[1], $codeMessage[2], $clientList[$user], $sock);
              break;
            case "UPWB":
              updateWhiteBoard($codeMessage[0], $codeMessage[1], $clientList, $sock);
              break;
            case "SVWB":
              saveWhiteBoard($codeMessage[0], $codeMessage[1], $sock);
              break;
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
              addToSide2($codeMessage[0], $codeMessage[1], $codeMessage[2], $ip, $clientList, $sock);
              break;
            case "FCFT":
              addToSide1($codeMessage[0], $codeMessage[1], $codeMessage[2], $ip, $clientList, $sock); // 0 = groupID 1 = card id 2 = message
              break;
          }//Switch Statement
      }//Closes else
    }//Closes foreach
}//Closes while
?>
