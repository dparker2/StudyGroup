<?php
include_once 'db_credentials.php';
include_once 'utilityFunctions.php';
include_once 'flashCardFunctions.php';
include_once 'groupFunctions.php';

function whiteboardLine($groupID, $point1, $point2, $ip, $clients, $sock) {
  $connection =  new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  // Check connection
  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);

  $return_ipList = "SELECT ipAddress FROM $groupID WHERE ipAddress IS NOT NULL";
  $resultIP = mysqli_query($connection, $return_ipList); //Returns list of current IP addresses i.e. current user list connected.
  $num_ip = $resultIP->num_rows; //Stores number of people currently connected for while loop iteration.
  while($num_ip > 0) { //Loops through each active client, printing out the current user list in order to update ui
    //Entire section below is to keep track of sockets we're writing to.
    $rowIP = mysqli_fetch_array($resultIP); //Fetches first IP as an array.
    //echo "Debugging: This is keyIP we're using to index: $rowIP[0] \n";
    $keyIP = $rowIP[0]; //Stores the key IP address:Port for use of $clients dict.
    $keySock = $clients[$keyIP][0]; //Uses the above to access the socket client of the IP address to write back to.
    if ($keySock != $sock) { // Makes sure doesn't write back to original socket.
      //echo "Debugging: This is keySock we're writing to: $keySock \n";
      $message = "WBLN$point1 $point2";
      $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT); //Pads left of code  with length of string so client knows how much to read
      fwrite($keySock,"{$messageSize}{$message}"); //Writes back to client.
      //echo "Debugging: Client should be receiving: {$messageSize}{$message} \n";
    }
    $num_ip = $num_ip - 1; //Goes to next IP address/User in group
  }//end while Loops
  $connection->close();
}

function updateWhiteBoard($ipaddress, $wbstring, $clients, $sock) {
  $newclient = $clients[$ipaddress][0];
  echo "DEBUG: This is the resource we should be writing to: $newclient \n";
  $message = "WBUP$wbstring";
  $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT);
  $submessage = substr($message,0, 10);
  echo "DEBUG: This is the string we're sending back {$messageSize}{$submessage}\n";
  fwrite($newclient, "{$messageSize}{$message}");
}

function saveWhiteBoard($groupID, $wbstring, $sock) {
  $connection =  new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  // Check connection
  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);
  $messageSize= str_pad((string)strlen($wbstring), 5, "0", STR_PAD_LEFT);
  echo "DEBUG: This is the string length before entering datbase: $messageSize ...\n";
  $escString = mysqli_escape_string($connection, $wbstring);
  $messageSizeEsc= str_pad((string)strlen($escString), 5, "0", STR_PAD_LEFT);
  echo "DEBUG: This is the esc string length before entering datbase: $messageSizeEsc ...\n";
  $saveWB = "UPDATE $groupID
             SET Whiteboard = '$escString'
             WHERE Whiteboard IS NOT NULL";
  mysqli_query($connection, $saveWB);

  if($connection->close()) {
    echo "Database closed\n";
  }
}
?>
