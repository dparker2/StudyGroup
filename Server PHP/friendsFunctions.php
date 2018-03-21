<?php
include_once 'db_credentials.php';
include_once 'sendEmail.php';
include_once 'utilityFunctions.php';
include_once 'classes.php';



function followUser($user, $follow, $sock) {
  $connection = connectAccountDB();
  $query = "INSERT INTO FriendTest (followerID, followedID) VALUES ('$user', '$follow')";
  if(mysqli_query($connection, $query)) {
    $message = "ADFRUser $follow has been successfully followed!\n\n";
    sendMessage($message, $sock);
  }
  else {
    $message = "FAILSomething went wrong.\n\n";
    sendMessage($message, $sock);
  }
  disconnectDB($connection);
}

function unfollowUser($user, $unfollow, $sock) {
  $connection = connectAccountDB();
  $query = "DELETE FROM FriendTest WHERE (followerID = '$user') AND (followedID = '$unfollow')";
  if(mysqli_query($connection, $query)) {
    $message = "RMFRUser $unfollow has been successfully unfollowed!\n\n";
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
    $joinedSearchResults = returnResultColumnAsStringSQL("Username", $connection, $query);
    $message = "RSLT$joinedSearchResults";
    sendMessage($message, $sock);
  }
  else {
    echo "Something went wrong... Error: " . mysqli_error($connection);
    $message = "FAIL";
    sendMessage($message, $sock);
  }

  disconnectDB($connection);
}

function returnFollowing($user, $sock) {
  $connection = connectAccountDB();
  $query = "SELECT Username FROM UserInfo WHERE UserID IN
              (SELECT f.followedID FROM FriendTest f INNER JOIN UserInfo u
                ON u.UserID = f.followerID WHERE followerID = 15)"; //returns usernames of users that $user follows
  if(mysqli_query($connection, $query)) {
    $joinedSearchResults = returnResultColumnAsStringSQL("Username", $connection, $query);
    $message = "FLNG$joinedSearchResults";
    sendMessage($message, $sock);
  }
  else {
    echo "Something went wrong... Error: " . mysqli_error($connection);
    $message = "FAIL";
    sendMessage($message, $sock);
  }
}









?>
