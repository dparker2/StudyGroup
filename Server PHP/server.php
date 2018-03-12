<?php
include_once 'accountFunctions.php';
include_once 'groupFunctions.php';
include_once 'flashCardFunctions.php';
include_once 'whiteboardFunctions.php';
include_once 'utilityFunctions.php';
include_once 'classes.php';
include_once 'friendsFunctions.php';
include_once 'homePageFunctions.php';

$server = stream_socket_server("tcp://0.0.0.0:9001", $errno, $errorMessage); //AWS EC2 server
//$server = stream_socket_server("tcp://localhost:1520", $errno, $errorMessage); //Localhost
//echo ++$argv[1];
//$_ = $_SERVER['_'];;
//echo "This is the server socket: ";
//var_dump($server);
echo "\n+++++++ Program Start +++++++\n";
if ($server[0] === false){
    die("Failed to bind to socket:  $errorMessage \n");
}
//Updates all users to offline on server startup in case of crash.
echo "Resetting all Online Users... \n";
if (clearAllOnlineStatusDB())
  echo "Reset done \n";
//Deletes all users in groups just in case of crash.
//Insures that no duplicate users would be printed
echo "Clearing Group Members...\n";
if(clearGroupMembersDB())
  echo "Clear done \n";

//Client streaming starts
$groupList = array();
$clientList = array();
while(true) {
    echo "\n+++++++++++++++++++++++++\n";
    echo "At top of the while loop, current clients connected: \n";
    echo "Clients:\n";
    var_dump($clientList);
    echo "Groups:\n";
    var_dump($groupList);

    //$read_socks = array_column($clients, 0);
    $read_socks = getSocketListCL($clientList);
    $read_socks["Server Socket: "] = $server;
    echo "Readable sockets:\n";
    var_dump($read_socks);
    echo "+++++++++++++++++++++++++\n \n";

    if(!stream_select ( $read_socks, $write, $except, 300000 ))
    {
        die('something went wrong while selecting');
    }

    if(in_array($server, $read_socks))
    {
        $new_client = stream_socket_accept($server);
        if ($new_client) {
            $user = stream_socket_get_name($new_client, true);
            echo "Connection accepted from $user \n";
            $clientList[$user] = new User();
            $clientList[$user]->setIP($user);
            $clientList[$user]->setSocket($new_client);
            echo "DEBUG: Verifying IP is " . $clientList[$user]->getIP() . "\n";
            echo "DEBUG: Verifying socket is " . $clientList[$user]->getSocket() ."\n";
        }
        unset($read_socks[ array_search($server, $read_socks) ]);
    }

    foreach($read_socks as $sock)
    {
        $data = fread($sock, 5);
        if ($data > 0) {
          $bytes = (int)$data;
          $messageLength = 0;
          $message = "";
          while(($messageLength < $bytes)){
            $newdata = fread($sock, ($bytes-$messageLength));
            $messageLength += strlen($newdata);
            $message = "{$message}{$newdata}";
          }
        }
        if(!$data)
        {
            $socketDC = array_search($sock, getSocketListCL($clientList), true);
            echo "DATA DOES NOT EXIST, $socketDC disconnected, closing socket...\n";
            var_dump(array_search($sock, getSocketListCL($clientList), true));
            logout($clientList[$socketDC]);
            unset($clientList[ array_search($sock, getSocketListCL($clientList), true)]);
            @fclose($sock);
            echo "Now there are total ". count($clientList) . " clients.\n";
            var_dump($clientList);
            continue;
        }
        else {
          $ip = stream_socket_get_name($sock, true);
          $client = $clientList[$ip];
          $code = substr($message, 0, 4);
          $msg = substr($message, 4, $bytes);

          if ($code == 'UPWB' || $code == 'SVWB' || $code == 'WBUP') {
            echo "DEBUG: Message Received  $bytes $code: WB String too long to echo. \n\n";}
          else if ($code == 'WBLN') {
            //nothing because it would be echo'd too many times because each point is going to be sent over.
          }
          else {
            echo "DEBUG: Message Received $bytes $message \n\n"; }

          $limit = 3;
          if ($code == "GCHT" || $code == "SVWB") {
            $limit = 2;
          }
          $codeMessage = explode(" ", $msg, $limit);

          switch($code) {
            case "CACC":
              createAccount($codeMessage[0], $codeMessage[1], $codeMessage[2], $sock);
              break;
            case "LOGN":
              loginAccount($codeMessage[0], $codeMessage[1], $client, $sock);
              break;
            case "LOGT":
              logout($client);
              break;
            case "CGRP":
              createGroup($codeMessage[0], $client, $clientList, $sock);
              break;
            case "JGRP":
              joinGroup($codeMessage[0], $client, $clientList, $sock);
              break;
            case "LGRP":
              leaveGroup($codeMessage[0], $client, $clientList, $sock);
              break;
            case "NWFG":
              newFavoriteGroup($client, $codeMessage[0]);
              break;
            case "RMFG":
              removeFavoriteGroup($client, $codeMessage[0]);
              break;
            case "GCHT":
              sendChatMessage($codeMessage[0], $codeMessage[1], $client, $clientList, $sock);
              break;
            case "WBLN":
              whiteboardLine($codeMessage[0], $codeMessage[1], $codeMessage[2], $clientList, $sock);
              break;
            case "UPWB":
              updateWhiteBoard($codeMessage[0], $codeMessage[1], $codeMessage[2], $clientList, $sock);
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
              if(count($codeMessage) < 3) {$codeMessage[2] = "";}
              addToCard($codeMessage[0], $codeMessage[1], $codeMessage[2], $client, $clientList, $sock, 2);
              break;
            case "FCFT":
              if(count($codeMessage) < 3) {$codeMessage[2] = "";}
              addToCard($codeMessage[0], $codeMessage[1], $codeMessage[2], $client, $clientList, $sock, 1);
              break;
            case "SUSR":
              searchUser($codeMessage[0], $sock);
              break;
          }
      }
    }
}
?>
