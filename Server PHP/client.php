<?php

//Used to test codes without having to use client.
$server = fsockopen("localhost", 1520, $errno, $errstr, 30);

if (!$server)
{
    echo "$errstr ($errno)\n";
}
else
{
    $out = "00019LOGNfuller testpass";
    ///Send data
    fwrite($server, $out);

    //fread($server, 1096);
    ///Receive data - in small chunks :)
    while (!feof($server))
    {
        echo fread($server, 4096);
    }

    fclose($server);
}
?>
