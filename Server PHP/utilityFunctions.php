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


class user {
  function __construct() {
    echo "Constructing";
  }
  function __destruct() {
    echo "Destructing";
  }
  function getName() {
    return $username;
  }
  function getIP() {
    return $ip;
  }
  function getSocket() {
    return $socket;
  }
  var $username;
  var $ip;
  var $socket;
  var $email;
}
?>
