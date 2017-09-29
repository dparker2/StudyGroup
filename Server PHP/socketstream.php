<?php
include 'functions.php';
//include 'rdsconnect.php';
// Opening a server that accepts anything from ports 9001
/*
$servername = "csci150-mysql-sg.cvawt8ol1m2q.us-east-2.rds.amazonaws.com";
$username = "admin";
$password = "csci1502017";
$dbname = "StudyGroup";
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
  if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
  }
  echo "Connected successfully to the Database as Well\n";*/

$server = stream_socket_server("tcp://0.0.0.0:9001", $errno, $errorMessage);

//Returns error message if we fail to bind
if ($server === false)
{
    die("Failed to bind to socket:  $errorMessage \n");
}

$clients = array();
while(true)
{
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
          echo "THIS IS YOUR MESSAGE: $data";
          $loginArray = explode(" ", $data);
          /*
          echo $loginArray[0]."\n";   //Code for creating new user account
          echo $loginArray[1]."\n";   //username
          echo $loginArray[2]."\n";   //password
          echo $loginArray[3]."\n";   //email*/

          if ($loginArray[0] == "CREATE") {
            createAccount($loginArray[1], $loginArray[2], $loginArray[3], $sock);
          }
          elseif ($loginArray[0] == "LOGIN") {
            loginAccount($loginArray[1], $loginArray[2], $sock);
          }
        }
    }
}
?>
