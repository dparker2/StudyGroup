<?php
include_once 'db_credentials.php';
include_once 'utilityFunctions.php';
include_once 'classes.php';


//Preps and sends message to update recent groups to client-side for homepage
function updateRecentGroups($user, $groupList) {
  $sock = $user->getSocket();
  if (count($user->getRecentGroups()) > 0) {
    $recentGroup = $user->getRecentGroups();
    $favoriteGroup = $user->getFavoriteGroups();
    $message = "UPRG";
    $messageArray = array();
    foreach ($recentGroup as $group) {
      array_push($messageArray, $group);
      if(in_array($group, $favoriteGroup))
        array_push($messageArray, '1');
      else
        array_push($messageArray, '0');
    }
    $implodeArray = implode(" ", $messageArray);
    $message .= $implodeArray;
    echo "DEBUG: Message for UPRG: $message \n";
    sendMessage($message, $sock);
  }
  else {
    $message = "UPRG";
    sendMessage($message, $sock);
  }
}

//Preps and sends message to update favorite groups to client-side for homepage
function updateFavoriteGroups($user, $groupList) {
  $sock = $user->getSocket();
  if (count($user->getFavoriteGroups()) > 0) {
    $favoriteGroup = $user->getFavoriteGroups();
    $message = "UPFG";
    $messageArray = array();
    foreach ($favoriteGroup as $group) {
      array_push($messageArray, $group);
    }
    $implodeArray = implode(" ", $messageArray);
    $message .= $implodeArray;
    echo "DEBUG: Message for UPFG: $message \n";
    sendMessage($message, $sock);
  }
  else {
    $message = "UPFG";
    sendMessage($message, $sock);
  }
}

//Adds group to favorite list.  Returns fail if group was already in list, or doesn't exist.
function newFavoriteGroup($user, $groupID) {
  global $groupList;
  $username = $user->getName();
  $sock = $user->getSocket();
  $groupExists = "SELECT TABLE_NAME FROM information_schema.TABLES
                  where TABLE_NAME = '$groupID'";
  $connection = connectGroupDB();
  if (checkExistsDB($connection, $groupExists) > 0) {
    $favGroups = $user->getFavoriteGroups();
    if (in_array($groupID, $favGroups)) {
      $message = "FAIL";
      sendMessage($message, $sock);
    }
    else {
      $user->setFavGroup($groupID);
      sendMessage("SUCC", $sock);
      updateFavoriteGroups($user, $groupList);
      updateRecentGroups($user, $groupList);
      $currentGroups = $user->getCurrGroups();
      if (in_array($groupID, $currentGroups)) {
        $message = "NWFG"."$groupID";
        sendMessage($message, $sock);
      }
    }
  }
  else {
    $message = "FAILGroup does not exist";
    sendMessage($message, $sock);
  }
  disconnectDB($connection);
}

//Removes a group from favorite list. Returns fail if group didn't exist in list.
function removeFavoriteGroup($user, $groupID) {
  global $groupList;
  $sock = $user->getSocket();
  $username = $user->getName();
  $favGroups = $user->getFavoriteGroups();
  if(!in_array($groupID, $favGroups)) {
    sendMessage("FAIL", $sock);
  }
  else {
    $user->removeFavGroup($groupID);
    sendMessage("SUCC", $sock);
    updateFavoriteGroups($user, $groupList);
    updateRecentGroups($user, $groupList);
    $currentGroups = $user->getCurrGroups();
    if (in_array($groupID, $currentGroups)) {
      $message = "RMFG"."$groupID";
      sendMessage($message, $sock);
    }
  }
}

?>
