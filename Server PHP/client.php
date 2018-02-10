<?php

//Used to test codes without having to use client.
$server = fsockopen("localhost", 1520, $errno, $errstr, 30);

function sendMessage($message, $socket) {
  $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT);
  echo "DEBUG: This is the fwrite: $message \n";
  fwrite($socket, "{$messageSize}{$message}");
}

if (!$server)
{
    echo "$errstr ($errno)\n";
}
else
{
    $out = "LOGNfuller testpass";
    sendMessage($out, $server);
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
        $newestdata = "{$newestdata}{$newdata}";
      }
    $code = substr($newestdata, 0, 4);
    $message = substr($newestdata, 4, $bytes);
    $in = fread($server, 1096);
    echo $in;
    if ($in):








    fclose($server);
}
?>
