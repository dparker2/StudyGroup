<?php

include_once 'db_credentials.php';
include_once 'utilityFunctions.php';
include_once 'sendEmail.php';


function reqSecQuest($user, $sock) { /*why not combine this function to take multiple parameters? then you could re-use function.
  maybe like another input determining how many questions to send and if it's just one, run randomizer? so like sendquest1, 2, 3, etc
  */
  $connection = connectAccount();
  $username = $user; //why username = user? why not just change parameter to username?? FOR ALL
  echo "the username input to the function is: $username\n\n";
  $query1 = "SELECT SQ1 FROM UserInfo WHERE Username = '$username'";
  echo "This is question1 query: $query1";
  $query2 = "SELECT SQ2 FROM UserInfo WHERE Username = '$username'";
  echo "This is question2 query: $query2";
  $query3 = "SELECT SQ3 FROM UserInfo WHERE Username = '$username'";
  echo "This is question3 query: $query3";

  /* not working for now, think it's a getobj string error
  $q1 = mysqli_query($connection, $query1);
  echo "q1 is: $q1\n\n";
  $q2 = mysqli_query($connection, $query2);
  echo "q2 is: $q2\n\n";
  $q3 = mysqli_query($connection, $query3);
  echo "q3 is: $q3\n\n";
  */

  $q1 = getObjString($connection, $query1)->SQ1;
  echo "This is q1 as string: " . "$q1\n";
  $q2 = getObjString($connection, $query2)->SQ2;
  echo "This is q2 as object? : " . "{$q2}\n\n";
  $q3 = getObjString($connection, $query3)->SQ3;
  echo "This is q3 as object? : " . "{$q3}\n\n";

  $message = "REQQ{$q1} {$q2} {$q3}";
  sendMessage($message, $sock);
  disconnect($connection);
}


//might actually not need this function but i'm leaving it for now
function reqSecAns($user, $sock) {
  $connection = connectAccount();
  $username = $user;
  $query1 = "SELECT SQA1 FROM UserInfo WHERE Username = '$username'";
  $query2 = "SELECT SQA2 FROM UserInfo WHERE Username = '$username'";
  $query3 = "SELECT SQA3 FROM UserInfo WHERE Username = '$username'";

  $a1 = getObjString($connection, $query1)->SQA1;
  $a2 = getObjString($connection, $query2)->SQA2;
  $a3 = getObjString($connection, $query3)->SQA3;

  $message = "REQA$a1 $a2 $a3";
  sendMessage($message, $sock);
  disconnect($connection);
}

function setSecQuest($user, $q1, $q2, $q3, $sock) {
  $connection = connectAccount();
  $username = $user;
  echo "This is q1 as received by setSecQuest: $q1\n\n";
  echo "This is q2 as received by setSecQuest: $q2\n\n";
  echo "This is q3 as received by setSecQuest: $q3\n\n";
  $escQ1 = mysqli_real_escape_string($connection, $q1);
  $escQ2 = mysqli_real_escape_string($connection, $q2);
  $escQ3 = mysqli_real_escape_string($connection, $q3);
  echo "This is escq1: $escQ1\n\n";
  echo "This is escq2: $escQ2\n\n";
  echo "This is escq3: $escQ3\n\n";
  $query = "UPDATE UserInfo SET SQ1='$escQ1', SQ2='$escQ2', SQ3='$escQ3' WHERE Username = '$username'";
  echo "Here is the setSecQuest query: $query\n\n";
  if (!mysqli_query($connection, $query)) {
    $message = "FAILThere was an issue setting the security questions.\n\n";
    echo("Error description: " . mysqli_error($connection));
    sendMessage($message, $sock);
  }
}



function setSecAns($user, $a1, $a2, $a3, $sock) {
  $connection = connectAccount();
  $username = $user;


  /*not sure if i need to escape string, or if hashing will be good enough. leaving this in case hashing alone does not work
  $escA1 = mysqli_real_escape_string($connection, $a1);
  $escA2 = mysqli_real_escape_string($connection, $a2);
  $escA3 = mysqli_real_escape_string($connection, $a3);
  */

  //$a1Hash = mysqli_real_escape_string($connection, password_hash($a1, PASSWORD_DEFAULT));
  //$a2Hash = mysqli_real_escape_string($connection, password_hash($a2, PASSWORD_DEFAULT));
  //$a3Hash = mysqli_real_escape_string($connection, password_hash($a3, PASSWORD_DEFAULT));

  $a1Hash = password_hash($a1, PASSWORD_DEFAULT);
  $a2Hash = password_hash($a2, PASSWORD_DEFAULT);
  $a3Hash = password_hash($a3, PASSWORD_DEFAULT);



  $query = "UPDATE UserInfo SET SQA1='$a1Hash', SQA2='$a2Hash', SQA3='$a3Hash' WHERE Username = '$username'";
  if (!mysqli_query($connection, $query)) {
    $message = "FAILThere was an issue setting the security question answers.\n\n";
    sendMessage($message, $sock);
  }

}

function sendRandomSecQuest($user, $sock) {
  $connection = connectAccount();
  $num = rand(1,3);
  $column = "SQ" . "$num";
  $query = "SELECT $column FROM UserInfo WHERE (Username='$user')";
  if (!mysqli_query($connection, $query)) {
    $message = "FAILUser doesn't exist or questions are not set.\n\n";
    sendMessage($message, $sock);
  }
  else {
    $question = getObjString($connection, $query)->$column;
    $message ="RPWD$num $question";
    sendMessage($message, $sock);
  }
  disconnect($connection);
}

function checkSecAnswer($user, $num, $answer, $sock) {
  $connection = connectAccount();
  $column = "SQA" . "$num";
  $query = "SELECT $column FROM UserInfo WHERE (Username='$user')";
  if(!mysqli_query($connection, $query)) {
    $message = "FAILUser doesn't exist or answer not set.\n\n";
    sendMessage($message, $sock);
  }
  else {
    $answerDB = getObjString($connection, $query)->$column;
    if (password_verify($answer, $answerDB)) {
      $query = "SELECT Email FROM UserInfo WHERE (Username='$user')";
      $email = getObjString($connection, $query)->Email;
      sendRecCode($email, $user);
      $message = "SUCCAn email has been sent to you with a recovery code. Please remember to check your spam folder!\n\n";
      sendMessage($message, $sock);
    }
    else {
      $message = "FAILAnswer did not match. Try again!";
      sendMessage($message, $sock);
    }
  }
}


function checkCode($user, $code, $sock) {
  $connection = connectAccount();
  $query = "SELECT RecCode FROM UserInfo WHERE (Username='$user')";
  if(!mysqli_query($connection, $query)) {
    $message = "FAIL";
    sendMessage($message, $sock);
  }
  else {
    if ($code == getObjString($connection, $query)->RecCode) {
      $message = "SUCC";
      sendMessage($message, $sock);
    }
    else {
      $message = "FAIL";
      sendMessage($message, $sock);
    }
  }
  disconnect($connection);
}


?>
