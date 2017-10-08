<?php
include 'accountFunctions.php';
include 'groupFunctions.php';

$server = stream_socket_server("tcp://0.0.0.0:9001", $errno, $errorMessage);

//Returns error message if we fail to bind
if ($server === false)
{
    die("Failed to bind to socket:  $errorMessage \n");
}

$clients = array();
while(true) {
    echo "Listening \n";
    //prepare readable sockets
    $read_socks = $clients;
    $read_socks[] = $server;

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
            $clients[] = $new_client;
        }
        //delete the server socket from the read sockets
        unset($read_socks[ array_search($server, $read_socks) ]);
    }

    //message from existing client
    foreach($read_socks as $sock)
    {
        $data = fread($sock, 128);
        if(!$data)
        {
            unset($clients[ array_search($sock, $clients) ]);
            @fclose($sock);
            echo "A client disconnected. Now there are total ". count($clients) . " clients.\n";
            continue;
        }
        //send the message back to client
        else {
          echo "THIS IS YOUR MESSAGE: $data \n";
          $loginArray = explode(" ", $data);

          if ($loginArray[0] == "CREATE") {
            createAccount($loginArray[1], $loginArray[2], $loginArray[3], $sock);
          }
          elseif ($loginArray[0] == "LOGIN") {
            loginAccount($loginArray[1], $loginArray[2], $sock);
          }
          elseif ($loginArray[0] == "LOGOUT") {
            logoutAccount($loginArray[1], $sock);
          }
          elseif ($loginArray[0] == "CREATEGRP") {
            createGroup($loginArray[1], $loginArray[2], $sock);
          }
          elseif ($loginArray[0] == "JOINGRP") {
            joinGroup($loginArray[1], $loginArray[2], $sock);
          }
        }
    }
}
?>
