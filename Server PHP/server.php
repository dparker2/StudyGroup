<?php
include_once 'accountFunctions.php';
include_once 'groupFunctions.php';
include_once 'flashCardFunctions.php';
include_once 'whiteboardFunctions.php';
include_once 'utilityFunctions.php';

// $socket_tuple = ($socket, $username)
//$server = stream_socket_server("tcp://0.0.0.0:9001", $errno, $errorMessage); //AWS EC2 server
$server = stream_socket_server("tcp://127.0.0.1:1520", $errno, $errorMessage); //Localhost
//YOLO
//Showing test commit hello
//echo ++$argv[1];
//$_ = $_SERVER['_'];;
echo "This is the server socket: ";
var_dump($server);
echo "\n+++++++ Program Start +++++++\n";
/*
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
$makeoffline = "UPDATE UserInfo
	   SET Status = 'Offline'
	   WHERE Status = 'Online'";
mysqli_query($connection, $makeoffline);

//Deletes all users in groups just in case of crash.
//Insures that no duplicate users would be printerd.
$countGroups = "SELECT TABLE_NAME
              FROM information_schema.columns
              where table_schema = 'StudyGroup'
              and column_name = 'userList';";
$resultGroups = mysqli_query($connection, $countGroups);
$numGroups = $resultGroups->num_rows;
echo "Number of groups in database: $numGroups \n";
while ($numGroups > 0) {
  $gArray = mysqli_fetch_array($resultGroups);
  $gName = $gArray[0];
  $removeNULL = "DELETE FROM $gName WHERE userList IS NOT NULL";
  //echo "$numGroups ";
  //echo "$removeNULL \n";
  $numGroups = $numGroups - 1;
  mysqli_query($connection, $removeNULL);
}
disconnect($connection);

$clients = array(); // $ip => ($socket, $username)
while(true) {
    //echo "Listening \n";
    //prepare readable sockets
    echo "Clients:\n";
    var_dump($clients);

    $read_socks = array_column($clients, 0);
    echo "Dumping read socks before setting server\n";
    var_dump($read_socks);
    $read_socks[] = $server;

    echo "Readable sockets:\n";
    var_dump($read_socks);

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

            //Assigns ip address and blank username to the array
            $clients[stream_socket_get_name($new_client, true)] = array($new_client, "");
            /*
            $user = stream_socket_get_name($new_client, true);
            $clients[$user] = new user();
            $clients[$user]->setIP(stream_socket_get_name($new_client, true));
            $clients[$user]->setSocket($new_client);

            echo "DEBUG: Verifying IP is " . $clients[$user]->getIP() . "\n";
            echo "DEBUG: Verifying socket is " . $clients[$user]->getSocket() ."\n"; */
        }
        //delete the server socket from the read sockets
        unset($read_socks[ array_search($server, $read_socks) ]);
    } //close if(in_array)

    //message from existing client
    foreach($read_socks as $sock)
    {
        // We go through every client socket here.
        // $ip holds the ip address of the client socket
        // $clients[$ip][1] is equal to the username AFTER successful login

        $data = fread($sock, 5);
        if ($data > 0) {
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
            var_dump(array_search($sock, array_column($clients, 0)));  //Array column returns values of index 0 of clients[], which is the socket.  array search looks at the list of sockets array_column returned for the $sock to close. This searches array(1)? printing out int(0)
            unset($clients[ array_search($sock, array_map(function($item){return $item[0];}, $clients)) ]);  //array_map applies the function return $item[0] to all clients.  so I guess that means it's returning the socket for all clients...? then it compares the sockets returned with array search, then the socket that matches array_search gets unset, it deletes it from the array. This searches array(2)?
            @fclose($sock); //closes the socket.
            echo "A client disconnected. Now there are total ". count($clients) . " clients.\n";
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
          //$ip = $clients[$user]->getIP();
          $ip = stream_socket_get_name($sock, true);
          //Takes in the first 5 bytes as to determine length of message.
          $code = substr($newestdata, 0, 4);
          $message = substr($newestdata, 4, $bytes);
          if ($code == 'UPWB' || $code == 'SVWB') {
            echo "THIS IS THE MESSAGE: {$bytes}{$code}: WB String too long to put. \n"; }
          else if ($code == 'WBLN') {
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
                $clients[$ip][1] = $codeMessage[0]; // Set username to clients dict
              } break;
            case "LOGT":
              logoutAccount($clients[$ip][1], $sock);
              break;
            case "CGRP":
              createGroup($codeMessage[0], $ip, $clients, $sock);
              break;
            case "JGRP":
              joinGroup($codeMessage[0], $ip, $clients, $sock);
              break;
            case "LGRP":
              leaveGroup($codeMessage[0], $ip, $clients, $sock);
              break;
            case "GCHT":
              sendChatMessage($codeMessage[0], $codeMessage[1], $ip, $clients, $sock);
              break;
            case "WBLN":
              whiteboardLine($codeMessage[0], $codeMessage[1], $codeMessage[2], $ip, $clients, $sock);
              break;
            case "UPWB":
              updateWhiteBoard($codeMessage[0], $codeMessage[1], $clients, $sock);
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
            case "RARQ":
              recoveryQset($codeMessage[0], $codeMessage[1], $sock);
              break;
            case "RUSR":
              rememberUsername($codeMessage[0], $sock);
              break;
            case "RUSP":
              rememberPassword($codeMessage[0], $codeMessage[1], $sock);
              break;
            case "FCBK":
              addToSide2($codeMessage[0], $codeMessage[1], $codeMessage[2], $ip, $clients, $sock);
              break;
            case "FCFT":
              addToSide1($codeMessage[0], $codeMessage[1], $codeMessage[2], $ip, $clients, $sock); // 0 = groupID 1 = card id 2 = message
              break;
          }//Switch Statement
      }//Closes else
    }//Closes foreach
}//Closes while
?>
