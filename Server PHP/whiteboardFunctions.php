<?php
include_once 'db_credentials.php';
include_once 'utilityFunctions.php';
include_once 'flashCardFunctions.php';
include_once 'groupFunctions.php';
include_once 'classes.php';

//WHITEBOARD LINE FUNCTION
//Takes in point1 and point2 of whiteboard and writes to every member of group for real time drawing
function whiteboardLine($groupID, $point1, $point2, $clientList, $sock) {
  global $groupList;
  $ipList = $groupList[$groupID]->getMemberIP();
  foreach($ipList as $ip) {
    $socket = $clientList[$ip]->getSocket();
    if($socket != $sock) {
      $message = "WBLN$groupID $point1 $point2";
      sendMessage($message, $socket);
    }
  }
}

//UPDATE WHITEBOARD FUNCTION
//Takes in the ipaddress requesting current whiteboard and returns whiteboard to update.
function updateWhiteBoard($ipaddress, $groupID, $wbstring, $clientList, $sock) {
  echo "Sending WB Update to new member...\n";
  $requestingClient = $clientList[$ipaddress]->getSocket();
  $message = "WBUP$groupID $wbstring";
  sendMessage($message, $requestingClient);
}

//SAVE WHITEBOARD FUNCTION
//Saves whiteboard onto database as string.
function saveWhiteBoard($groupID, $wbstring, $sock) {
  $connection = connectGroupDB();
  $escString = mysqli_escape_string($connection, $wbstring);
  echo "Saving Whiteboard into Database...\n";
  $saveWB = "UPDATE $groupID
             SET Whiteboard = '$escString'
             WHERE Whiteboard IS NOT NULL";
  mysqli_query($connection, $saveWB);
  disconnectDB($connection);
}
?>
