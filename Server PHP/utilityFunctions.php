<?php
include_once 'db_credentials.php';
include_once 'classes.php';

function checkExistsDB($connection, $query) {
  if ($stmt = mysqli_prepare($connection, $query)) {
    mysqli_stmt_execute($stmt);
    mysqli_stmt_store_result($stmt);
    $exists = mysqli_stmt_num_rows($stmt);
    mysqli_stmt_close($stmt);
    return $exists;
  }
}

function connectGroupDB() {
  $connection = new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);
  else
    echo "Connected to database \n";
  return $connection;
}

function connectAccountDB() {
  $connection = new mysqli(DB2_Server, DB2_User, DB2_Pass, DB2_Name);
  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);
  else
    echo "Connected to database \n";
  return $connection;
}

function disconnectDB($connection) {
  if($connection->close())
    echo "Database Closed \n";
}

function clearGroupMembersDB() {
  $connection = connectGroupDB();
  $countGroups = "SELECT TABLE_NAME
                FROM information_schema.columns
                where table_schema = 'StudyGroup'
                and column_name = 'userList';";
  $groupsWithUsers = mysqli_query($connection, $countGroups);
  while ($gArray = mysqli_fetch_array($groupsWithUsers)) {
    $gName = $gArray[0];
    $removeNULL = "DELETE FROM $gName WHERE userList IS NOT NULL";
    mysqli_query($connection, $removeNULL);
  }
  disconnectDB($connection);
  return true;
}

function clearFromGroupsGL($user, $connection) {
  global $groupList;
  global $clientList;
  $groupArray = $user->getCurrGroups();
  $username = $user->getName();
  foreach($groupArray as $group) {
    $groupClass = $groupList[$group];
    $groupClass->removeMember($user->getName());
    updateGroupList($connection, $clientList, $groupClass, $group);
  }
}

function clearAllOnlineStatusDB() {
  $dbAccount = connectAccountDB();
  $offline = "UPDATE UserInfo
                  SET UserStatus = 'Offline'
                  WHERE UserStatus = 'Online'";
  mysqli_query($dbAccount, $offline);
  disconnectDB($dbAccount);
  return true;
}

function setOfflineDB($user, $connection) {
  $username = $user->getName();
  $offline = "UPDATE UserInfo
                  SET UserStatus = 'Offline'
                  WHERE UserStatus = 'Online' AND Username = '$username'";
  mysqli_query($connection, $offline);
}

function setOfflineCL($user) {
  $user->setName(NULL);
  $user->setEmail(NULL);
  $user->clearGroups();
}

function setFavoriteGroupsDB($user, $connection) {
  $username = $user->getName();
  if (count($user->getFavoriteGroups()) > 0) {
    echo "Saving favorite groups for $username\n";
    $favGroup = implode(" ", $user->getFavoriteGroups());
    echo "$favGroup \n";
    $set_favorite_groups = "UPDATE UserInfo SET FavoriteGroups='$favGroup' WHERE Username = '$username'";
    mysqli_query($connection, $set_favorite_groups);
  }
  else {
    $set_favorite_groups = "UPDATE UserInfo SET FavoriteGroups=NULL WHERE Username = '$username'";
    mysqli_query($connection, $set_favorite_groups);
  }
}

function setRecentGroupsDB($user, $connection) {
  $username = $user->getName();
  if (count($user->getRecentGroups()) > 0) {
    echo "Saving recent groups for $username \n";
    $recGroup = implode(" ", $user->getRecentGroups());
    echo "$recGroup \n";
    $set_recent_groups = "UPDATE UserInfo SET RecentGroups='$recGroup' WHERE Username = '$username'";
    mysqli_query($connection, $set_recent_groups);
  }
  else {
    $set_recent_groups = "UPDATE UserInfo SET RecentGroups=NULL WHERE Username = '$username'";
    mysqli_query($connection, $set_recent_groups);
  }
}

function getObjStringDB($connection, $query) {
  $queryResult = mysqli_query($connection, $query);
  $object = $queryResult->fetch_object();
  return $object;
}

function sendMessage($message, $socket) {
  $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT);
  $code = substr($message, 0, 4);
  if ($messageSize < 100 && $code != 'WBLN')
    echo "DEBUG: This is the fwrite: $messageSize $message \n";
  fwrite($socket, "{$messageSize}{$message}");
}


function getSocketListCL($clientList) {
  $clientNum = count($clientList);
  $socketList = array();
  foreach ($clientList as $user) {
    $socketList[$user->getIP()] = $user->getSocket();
  }
  return $socketList;
}
?>
