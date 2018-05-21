<?php
include_once 'db_credentials.php';
include_once 'sendEmail.php';
include_once 'utilityFunctions.php';
include_once 'classes.php';



function followUser($followID, $user, $clientList, $sock) {
  $connection = connectAccountDB();
  $username = $user->getName();
  $userID = $user->getID();
  $followUsername = $clientList[$followID]->getName();
  $user->addFollowing($followUsername, $followID);
  $message = "ADFRUser " . $followUsername " has been successfully followed!";
  sendMessage($message, $sock);
  $clientList[$followID]->addFollowers($username, $userID);
  $message = "SUCC" . $followUsername . "'s followers array has been updated!";
  disconnectDB($connection);
  //at this point, not sure what to do? get with Dave about how to let $followID know followers was updated... or IF they need to know
}

function unfollowUser($unfollowID, $user, $clientList, $sock) {
  $connection = connectAccountDB();
  $userID = $user->getID();
  $unfollowUsername = $clientList[$unfollowID]->getName(); //only for debug, remove once working
  $user->removeFollowing($unfollowID);
  $message = "RMFRUser $unfollowUsername has been successfully unfollowed!\n\n";
  sendMessage($message, $sock);
  $clientList[$unfollowID]->removeFollowers($userID);
  $message = "SUCC" . $unfollowUsername . "'s followers array has been updated!"; // same as above.. talk to dave
  disconnectDB($connection);
}

function searchUser($user, $sock) {
  $connection = connectAccountDB();
  $query = "SELECT Username FROM UserInfo WHERE Username LIKE '%$user%' ORDER BY Username ASC LIMIT 20";
  if(mysqli_query($connection, $query)) {
    $joinedSearchResults = getResultColumnAsStringDB("Username", $connection, $query);
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

function returnFollowingListDB($user, $sock) {
  $connection = connectAccountDB();
  $query = "SELECT Username, UserID FROM UserInfo WHERE UserID IN
              (SELECT f.followedID FROM FriendTest f INNER JOIN UserInfo u
                ON u.UserID = f.followerID WHERE followerID = '$user')"; //returns usernames & ids of users that $user follows
  if(mysqli_query($connection, $query)) {
    $joinedSearchResults = getResultTwoColumnsAsStringDB("Username", "UserID", $connection, $query);
    $message = "FLNG$joinedSearchResults";
    sendMessage($message, $sock);
  }
  else {
    echo "Something went wrong... Error: " . mysqli_error($connection);
    $message = "FAIL";
    sendMessage($message, $sock);
  }
  disconnectDB($connection);
}

function returnFollowerListDB($user, $sock) {
  $connection = connectAccountDB();
  $query = "SELECT Username, UserID FROM UserInfo WHERE UserID IN
              (SELECT f.followerID FROM FriendTest f INNER JOIN UserInfo u
                ON u.UserID = f.followedID WHERE followedID = '$user')";
  if(mysqli_query($connection, $query)) {
    $joinedSearchResults = getResultTwoColumnsAsStringDB("Username", "UserID", $connection, $query);
    $message = "FLWR$joinedSearchResults";
    sendMessage($message, $sock);
  }
  else {
    echo "Something went wrong... Error: " . mysqli_error($connection);
    $message = "FAIL";
    sendMessage($message, $sock);
  }
  disconnectDB($connection);
}

function updateFollowingListDB($user, $sock) {
  $connection = connectAccountDB();
  /*
  will probably need to find all entries with followerID = $user and then just rewrite everything from user->followinglist
  */
  disconnectDB($connection);
}

function updateFollowersListDB($user, $sock) {
  $connection = connectAccountDB();
  /*
  will probably need to find all entries with followingID = $user and then just rewrite everything from user->followerslist
  */
  disconnectDB($connection);
}







?>
