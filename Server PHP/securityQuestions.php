<?php

include_once 'db_credentials.php';
include_once 'utilityFunctions.php';
include_once 'sendEmail.php';


function reqSecQuest($user, $sock) {
  $connection = connectAccount();
  $username = $user->getName();
  $q1 = "SELECT SQ1 FROM UserInfo WHERE Username = '$username'";
  $q2 = "SELECT SQ2 FROM UserInfo WHERE Username = '$username'";
  $q3 = "SELECT SQ3 FROM UserInfo WHERE Username = '$username'";

  $message = "REQQ$q1 $q2 $q3";
  sendMessage($message, $sock);
  disconnect($connection);
}


//might actually not need this function but i'm leaving it for now
function reqSecAns($user, $sock) {
  $connection = connectAccount();
  $username = $user->getName();
  $a1 = "SELECT SQA1 FROM UserInfo WHERE Username = '$username'";
  $a2 = "SELECT SQA2 FROM UserInfo WHERE Username = '$username'";
  $a3 = "SELECT SQA3 FROM UserInfo WHERE Username = '$username'";

  $message = "REQA$a1 $a2 $a3";
  sendMessage($message, $sock);
  disconnect($connection);
}

function setSecQuest($user, $q1, $q2, $q3, $sock) {
  $connection = connectAccount();
  $username = $user->getName();
  /*will only need these if client sends the server questions without spaces, but probably will not need
  $q1_nospaces = str_replace(' ','_',$q1);
  $q2_nospaces = str_replace(' ','_',$q2);
  $q3_nospaces = str_replace(' ','_',$q3);
  */

  $escQ1 = mysqli_real_escape_string($connection, $q1);
  $escQ2 = mysqli_real_escape_string($connection, $q2);
  $escQ3 = mysqli_real_escape_string($connection, $q3);
  $query = "UPDATE UserInfo SET SQ1='$escQ1', SET SQ2='$escQ2', SET SQ3='$escQ3' WHERE Username = '$username'";
  if (!mysqli_query($connection, $query)) {
    $message = "FAILThere was an issue setting the security questions.\n\n";
    sendMessage($message, $sock);
  }

}



function setSecAns($user, $a1, $a2, $a3, $sock) {
  $connection = connectAccount();
  $username = $user->getName();
  /*will only need these if client sends the server questions without spaces, but probably will not need
  $q1_nospaces = str_replace(' ','_',$q1);
  $q2_nospaces = str_replace(' ','_',$q2);
  $q3_nospaces = str_replace(' ','_',$q3);
  */

  /*not sure if i need to escape string, or if hashing will be good enough. leaving this in case hashing alone does not work
  $escA1 = mysqli_real_escape_string($connection, $a1);
  $escA2 = mysqli_real_escape_string($connection, $a2);
  $escA3 = mysqli_real_escape_string($connection, $a3);
  */

  $a1Hash = password_hash($a1, PASSWORD_DEFAULT);
  $a2Hash = password_hash($a2, PASSWORD_DEFAULT);
  $a3Hash = password_hash($a3, PASSWORD_DEFAULT);

  $query = "UPDATE UserInfo SET SQA1='$a1Hash', SET SQA2='$a2Hash', SET SQA3='$a3Hash' WHERE Username = '$username'";
  if (!mysqli_query($connection, $query)) {
    $message = "FAILThere was an issue setting the security question answers.\n\n";
    sendMessage($message, $sock);
  }

}

?>
