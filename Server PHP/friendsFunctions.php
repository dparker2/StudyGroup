<?php
include_once 'db_credentials.php';
include_once 'sendEmail.php';
include_once 'utilityFunctions.php';
include_once 'classes.php';

function followUser($user, $follow) {
  $connection = connectAccount();
  $query = "INSERT INTO FriendTest (followerID, followedID) VALUES ('$user', '$follow')";
  if(mysqli_query($connection, $query)) {
    $message = "SUCCUser $follow has been successfully followed!\n\n";
    sendMessage($message, $sock);
  }
  else {
    $message = "FAILSomething went wrong.\n\n";
    sendMessage($message, $sock);
  }
  disconnect($connection);
}

function searchUser($user, $sock) {
  $connection = connectAccount();
  $query = "SELECT Username FROM UserInfo WHERE Username LIKE '%$user%' ORDER BY Username ASC LIMIT 20";
  if(mysqli_query($connection, $query)) {
    echo "Yeeeeeeeee buddy it worked!\n\n";
    $result = mysqli_query($connection, $query);
    $searchResults = "";
    while ($rows = mysqli_fetch_array($result, MYSQLI_ASSOC)) {
      $searchResults .= $rows["Username"] . " ";
    }
    $message = "SUCC$searchResults";
    sendMessage($message, $sock);
  }
  else {
    echo "Damn it didn't work son :/ ... Error: " . mysqli_error($connection);
    $message = "FAIL";
  }

  disconnect($connection);
}




?>
