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
<<<<<<< HEAD
    $read_socks = $clients;
    $read_socks[] = $server;

=======
    //echo "Clients:\n";
    //var_dump($clients);

    $read_socks = array_column($clients, 0);
    $read_socks[] = $server;

    //echo "Readable sockets:\n";
    //var_dump($read_socks);

>>>>>>> f444aee96b88f9ea5f8c74e021c29fe863deb20f
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
<<<<<<< HEAD
        $data = fread($sock, 128);
=======
        // We go through every client socket here.
        // $ip holds the ip address of the client socket
        // $clients[$ip][1] is equal to the username AFTER successful login

        $data = fread($sock, 5);
        if ($data > 0) {
          $bytes = (int)$data;
          $newdata = fread($sock, $bytes);}
>>>>>>> f444aee96b88f9ea5f8c74e021c29fe863deb20f
        if(!$data)
        {
            unset($clients[ array_search($sock, $clients) ]);
            @fclose($sock);
            echo "A client disconnected. Now there are total ". count($clients) . " clients.\n";
            continue;
        }
        //send the message back to client
        else {
<<<<<<< HEAD
          echo "THIS IS YOUR MESSAGE: $data";
          $loginArray = explode(" ", $data);
=======
          $ip = stream_socket_get_name($sock, true);
>>>>>>> f444aee96b88f9ea5f8c74e021c29fe863deb20f

          //Takes in the first 5 bytes as to determine length of message.
          $code = substr($newdata, 0, 4);
          $message = substr($newdata, 4, $bytes);
          echo "THIS IS THE MESSAGE $code: $message \n";
          $loginArray = explode(" ", $message);  //Puts message into array

          if ($code == "CACC") {
            createAccount($loginArray[0], $loginArray[1], $loginArray[2], $sock);
          }
<<<<<<< HEAD
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
=======
          elseif ($code == "LOGN") {
            if(loginAccount($loginArray[0], $loginArray[1], $sock))
            {
              $clients[$ip][1] = $loginArray[0]; // Set username to clients dict
            }
          }
          elseif ($code == "LOGT") {
            logoutAccount($client[$ip][1], $sock);
          }
          elseif ($code == "CGRP") {
            createGroup($loginArray[0], $ip, $clients, $sock);
          }
          elseif ($code == "JGRP") {
            joinGroup($loginArray[0], $ip, $clients, $sock);
          }
          elseif ($code == "LGRP") {
            leaveGroup($loginArray[0], $ip, $clients, $sock);
          }
          elseif ($code == "CHPW") {
            changePassword($client[$ip][1], $loginArray[1], $sock);
>>>>>>> f444aee96b88f9ea5f8c74e021c29fe863deb20f
          }
          elseif ($code == "RACC") {
            recoverAccount($loginArray[0], $loginArray[1], $sock);
          }
          elseif ($code == "RARQ") {
              recoveryQset($loginArray[0], $loginArray[1], $sock);
        }
    }
}
?>
