<?php
include_once 'db_credentials.php';
include_once 'classes.php';
//Test function for existence.
function checkExists($connection, $query) {
  if ($stmt = mysqli_prepare($connection, $query)) {
    mysqli_stmt_execute($stmt);
    mysqli_stmt_store_result($stmt);
    $exists = mysqli_stmt_num_rows($stmt);
    mysqli_stmt_close($stmt);
    return $exists;
  }
}

function connectGroup() {
  $connection = new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);
  //else
    //echo "Connected to database \n";
  return $connection;
}

function connectAccount() {
  $connection = new mysqli(DB2_Server, DB2_User, DB2_Pass, DB2_Name);
  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);
  //else
    //echo "Connected to database \n";
  return $connection;
}

function disconnect($connection) {
  $connection->close();
  //if($connection->close()) {
    //echo "Database Closed \n";
}

function clearGroupMembers() {
  $connection = connectGroup();
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
  disconnect($connection);
  return true;
}

function clearFromGroup($user) {
  global $groupList;
  global $clientList;
  $connection = connectGroup();
  $groupArray = $user->getCurrGroups();
  $username = $user->getName();
  foreach($groupArray as $group) {
    $groupClass = $groupList[$group];
    $groupClass->removeMember($user->getName());
    /*$remFromGroup = "DELETE FROM $group WHERE userList = '$username'";
    mysqli_query($connection, $remFromGroup);*/
    updateGroupList($connection, $clientList, $groupClass, $group);
  }
  disconnect($connection);
}

function clearAllOnlineStatus() {
  $dbAccount = connectAccount();
  $makeoffline = "UPDATE UserInfo
                  SET UserStatus = 'Offline'
                  WHERE UserStatus = 'Online'";
  mysqli_query($dbAccount, $makeoffline);
  disconnect($dbAccount);
  return true;
}

function clearOnlineStatus($username) {
  $dbAccount = connectAccount();
  $makeoffline = "UPDATE UserInfo
                  SET UserStatus = 'Offline'
                  WHERE UserStatus = 'Online' AND Username = '$username'";
  mysqli_query($dbAccount, $makeoffline);
  disconnect($dbAccount);
}

function getNumRows($connection, $query) {
  $queryResult = mysqli_query($connection, $query);
  $numRows = $queryResult->num_rows;
  return $numRows;
}

function getObjString($connection, $query) {
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


function getSocketList($clientList) {
  $clientNum = count($clientList);
  //echo "DEBUG: In getSocketList, number of clients: ". $clientNum . "\n";
  $socketList = array();
  foreach ($clientList as $user) {
    $socketList[$user->getIP()] = $user->getSocket();
  }
  return $socketList;
}

?>
