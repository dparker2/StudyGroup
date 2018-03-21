<?php
include_once 'db_credentials.php';
include_once 'sendEmail.php';
include_once 'utilityFunctions.php';
include_once 'classes.php';

function followUser($user, $follow) {
  $connection = connectAccountDB();
  $query = "INSERT INTO FriendTest (followerID, followedID) VALUES ('$user', '$follow')";
  if(mysqli_query($connection, $query)) {
    $message = "SUCCUser $follow has been successfully followed!\n\n";
    sendMessage($message, $sock);
  }
  else {
    $message = "FAILSomething went wrong.\n\n";
    sendMessage($message, $sock);
  }
  disconnectDB($connection);
}

function searchUser($user, $sock) {
  $connection = connectAccountDB();
  $query = "SELECT Username FROM UserInfo WHERE Username LIKE '%$user%' ORDER BY Username ASC LIMIT 20";
  if(mysqli_query($connection, $query)) {
    $result = mysqli_query($connection, $query);
    $searchResults = [];
    while ($rows = mysqli_fetch_array($result, MYSQLI_ASSOC)) {
      array_push($searchResults, $rows["Username"]);
    }
    $joinedSearchResults = join(" ", $searchResults);
    $message = "RSLT$joinedSearchResults";
    sendMessage($message, $sock);
  }
  else {
    echo "Something went wrong... Error: " . mysqli_error($connection);
    $message = "FAIL";
  }

  disconnectDB($connection);
}




?>
