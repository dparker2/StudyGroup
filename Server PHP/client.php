<?php

//Used to test codes without having to use client.
$server = fsockopen("localhost", 1520, $errno, $errstr, 30);

function sendMessage($message, $socket) {
  $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT);
  echo "DEBUG: This is the fwrite: $message \n";
  fwrite($socket, "{$messageSize}{$message}");
}
function readMessage($server) {
  $data = fread($server, 5);
  if ($data > 0) {
    echo "Server sent back: \n";
    $bytes = (int)$data;
    $lennewdata = 0;
    $newestdata = "";
    while(($lennewdata < $bytes)){
      $newdata = fread($server, $bytes);
      $lennewdata = $lennewdata + strlen($newdata);
      $newestdata = "{$newestdata}{$newdata}";
    } echo "$newestdata \n";
    $code = substr($newestdata, 0, 4);
    $message = substr($newestdata, 4, $bytes);
  }
  return array($newestdata, $code, $message);
}
if (!$server)
{
    echo "$errstr ($errno)\n";
}
else
{
    $grpID = "placeholder";
    $group = "testgroup";
    $username = "wetestingexecution";
    $email = "wetesting@mail.com";
    $password = "testpass";

    $cacc = "CACC$email $username $password";
    $logn = "LOGN$username $password";
    $succLogn = "SUCC$email";
    $cgrp = "CGRP$group";
    $gcht = "GCHT$grpID test message";


    //while (!feof($server)) {
      echo "Waiting for data now \n\n";
      sendMessage($cacc, $server);
      $succCACC = readMessage($server);
      if($succCACC = "SUCC") {
        sendMessage($logn, $server);
        if (readMessage($server)[1] == "SUCC") {
          sendMessage($cgrp, $server);
          $grpCREATED = readMessage($server);
          if ($grpCREATED[1] = "SUCC") {
            $grpID = $grpCREATED[2];
            sendMessage("JGRP$grpID", $server);
            $jgrpSUCC = readMessage($server);
            echo $jgrpSUCC;
            /*if ($jgrpSUCC == "SUCC") {
              sendMessage($gcht, $server);
              $gchtSUCC = readMessage($server);*/
            //}
          }//Join group bracket
        }//Create group backet
      }//Login bracket
    //} //Ending server while loop









    fclose($server);
}
?>
