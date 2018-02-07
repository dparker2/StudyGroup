<?php

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

function connect() {
  $newConnect = new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  if ($newConnect->connect_error)
    die("Connection failed: " . $connect->connect_error);
  else
    echo "Connected to database \n";
  return $newConnect;
}

function connect2() {
  $newConnect = new mysqli(DB2_Server, DB2_User, DB2_Pass, DB2_Name);
  if ($newConnect->connect_error)
    die("Connection failed: " . $connect->connect_error);
  else
    echo "Connected to database \n";
  return $newConnect;
}

function disconnect($currConnect) {
  if($currConnect->close()) {
    echo "Database Closed \n";
  }
}

function clearGroupMembers($connection, $countGroups) {
  $groupsWithUsers = mysqli_query($connection, $countGroups);
  while ($gArray = mysqli_fetch_array($groupsWithUsers)) {
    $gName = $gArray[0];
    $removeNULL = "DELETE FROM $gName WHERE userList IS NOT NULL";
    mysqli_query($connection, $removeNULL);
  }
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
  echo "DEBUG: This is the fwrite: $message \n";
  fwrite($socket, "{$messageSize}{$message}");
}


function getSocketList($clientList) {
  $clientNum = count($clientList);
  echo "DEBUG: In getSocketList, number of clients: ". $clientNum . "\n";
  $socketList = array();
  foreach ($clientList as $user) {
    $socketList[$user->getIP()] = $user->getSocket();
  }
  return $socketList;
}
class User {
  function __construct() {
    echo "Constructing user class\n";
  }
  function __destruct() {
    echo "Destructing user class\n";
  }

  var $username;
  var $ip;
  var $socket;
  var $email;

  function getName() {
    return $this->username;
  }
  function getIP() {
    return $this->ip;
  }
  function getSocket() {
    return $this->socket;
  }
  function getEmail() {
    return $this->email;
  }
  function setName($Name) {
    $this->username = $Name;
  }
  function setIP($ipAddress) {
    $this->ip = $ipAddress;
  }
  function setSocket($sock) {
    $this->socket = $sock;
  }
  function setEmail($usrEmail) {
    $this->email = $usrEmail;
  }

}
?>
