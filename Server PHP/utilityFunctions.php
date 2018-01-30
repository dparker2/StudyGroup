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

function disconnect($currConnect) {
  if($currConnect->close()) {
    echo "Database Closed \n";
  }
}

function getNumRows($connection, $query) {
  $queryResult = mysqli_query($connection, $query);
  $numRows = $resultGroups->num_rows;
  return $numRows;
}

function getObjString($connection, $query) {
  $queryResult = mysqli_query($connection, $query);
  $object = $queryResult->fetch_object();
  return $object;
}

function sendMessage($message, $socket) {
  $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT);
  fwrite($socket, "{$messageSize}{$message}");
}

class user {
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
