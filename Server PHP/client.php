<?php
$fp = fsockopen("ec2-18-221-67-202.us-east-2.compute.amazonaws.com", 9001, $errno, $errstr, 30);

if (!$fp)
{
    echo "$errstr ($errno)<br />\n";
}
else
{
    $out = "00021LOGIN fuller testpass \n";
    ///Send data
    fwrite($fp, $out);

    ///Receive data - in small chunks :)
    while (!feof($fp))
    {
        echo fgets($fp, 128);
    }

    fclose($fp);
}
?>
