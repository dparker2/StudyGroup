<?php
include 'accountFunctions.php';
include 'groupFunctions.php';

// $socket_tuple = ($socket, $username)
$server = stream_socket_server("tcp://0.0.0.0:9001", $errno, $errorMessage);

//Returns error message if we fail to bind
if ($server[0] === false)
{
    die("Failed to bind to socket:  $errorMessage \n");
}

$clients = array(); // $ip => ($socket, $username)
while(true) {
    echo "Listening \n";
    //prepare readable sockets
    echo "Clients:\n";
    var_dump($clients);

    $read_socks = array_column($clients, 0);
    $read_socks[] = $server;

    echo "Readable sockets:\n";
    var_dump($read_socks);

    //start reading and use a large timeout
    if(!stream_select ( $read_socks, $write, $except, 300000 ))
    {
        die('something went wrong while selecting');
    }

    //new client
    if(in_array($server, $read_socks))
    {
        $new_client = stream_socket_accept($server);
        if ($new_client)
        {
            //print remote client information, ip and port number
            echo 'Connection accepted from ' . stream_socket_get_name($new_client, true) . "\n";
            $clients[stream_socket_get_name($new_client, true)] = array($new_client, "");
        }
        //delete the server socket from the read sockets
        unset($read_socks[ array_search($server, $read_socks) ]);
    }

    //message from existing client
    foreach($read_socks as $sock)
    {
        // We go through every client socket here.
        // $ip holds the ip address of the client socket
        // $clients[$ip][1] is equal to the username AFTER successful login

        $data = fread($sock, 128);
        if(!$data)
        {
            var_dump(array_search($sock, array_column($clients, 0)));
            unset($clients[ array_search($sock, array_map(function($item){return $item[0];}, $clients)) ]);
            @fclose($sock);
            echo "A client disconnected. Now there are total ". count($clients) . " clients.\n";
            continue;
        }
        //send the message back to client
        else {
          $ip = stream_socket_get_name($sock, true);
          echo "THIS IS YOUR MESSAGE: $data";
          //Takes in the first 5 bytes as to determine length of message.
          $bytes = (int)(substr($data, 0, 5));
          $message = substr($data, 5, $bytes);
          $loginArray = explode(" ", $message);  //Puts message into array

          if ($loginArray[0] == "CACC") {
            createAccount($loginArray[1], $loginArray[2], $loginArray[3], $sock);
          }
          elseif ($loginArray[0] == "LOGN") {
            if(loginAccount($loginArray[1], $loginArray[2], $sock))
            {
              $clients[$ip][1] = $loginArray[1]; // Set username to clients dict
            }
          }
          elseif ($loginArray[0] == "LOGT") {
            logoutAccount($client[$ip][1], $sock);
          }
          elseif ($loginArray[0] == "CGRP") {
            createGroup($loginArray[1], $ip, $clients, $sock);
          }
          elseif ($loginArray[0] == "JGRP") {
            joinGroup($loginArray[1], $ip, $clients, $sock);
          }
          elseif ($loginArray[0] == "CPAS") {
            changePassword($client[$ip][1], $loginArray[2], $sock);
          }
          elseif ($loginArray[0] == "RACC") {
            recoverAccount($loginArray[1], $sock);
          }
        }
    }
}
?>
