<?php
include_once 'db_credentials.php';
include_once 'utilityFunctions.php';
include_once 'flashCardFunctions.php';
include_once 'groupFunctions.php';
include_once 'classes.php';
/* Whiteboard Functions
  function whiteboardLine($groupID, $point1, $point2, $clientList, $sock)
  function updateWhiteBoard($ipaddress, $wbstring, $clientList, $sock)
  function saveWhiteBoard($groupID, $wbstring, $sock)
*/
//WHITEBOARD LINE FUNCTION
//Takes in point1 and point2 of whiteboard and writes to every member of group for real time drawing
function whiteboardLine($groupID, $point1, $point2, $clientList, $sock) {
  $connection = connectGroup();
  $returnIPList = "SELECT ipAddress FROM $groupID WHERE ipAddress IS NOT NULL";
  $ipList = mysqli_query($connection, $returnIPList);
  while($rowIP = mysqli_fetch_array($ipList)) { //Actively sends whiteboard to everyone in group
    $keyIP = $rowIP[0];                         //Stores IP for socket access
    $keySock = $clientList[$keyIP]->getSocket();//Uses IP to obtain socket that is going to be written to
    if ($keySock != $sock) {                    // Makes sure doesn't write back to original socket.
      $message = "WBLN$groupID $point1 $point2";
      sendMessage($message, $keySock);
    }//end if statement
  }  //end while Loops
  disconnect($connection);
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
  $connection = connectGroup();
  $escString = mysqli_escape_string($connection, $wbstring);
  echo "Saving Whiteboard into Database...\n";
  $saveWB = "UPDATE $groupID
             SET Whiteboard = '$escString'
             WHERE Whiteboard IS NOT NULL";
  mysqli_query($connection, $saveWB);
  disconnect($connection);
}
?>
