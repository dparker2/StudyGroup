<?php
// Create a new group given username input
include_once 'db_credentials.php';

function createGroup($groupname, $ip, $clients, $sock)
{
  // Create connection
  $connection =  new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);

  // Check connection
  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);
  else
    echo "Connected to database \n";

  $rand4 = rand(1000, 9999);
  $groupID = $groupname . "_" . $rand4;
  // sql command to check if groupname table already exists
  $check_groupID = "SELECT table_name FROM information_schema.tables WHERE TABLE_SCHEMA='StudyGroup' AND table_name='$groupID'";
  // Run, store, and close sql commands
  if ($stmt = mysqli_prepare($connection, $check_groupID)) {
    mysqli_stmt_execute($stmt);
    mysqli_stmt_store_result($stmt);
    $groupID_exists = mysqli_stmt_num_rows($stmt);
    mysqli_stmt_close($stmt);
  }

  //if group name exists, return failure. else, run query and create table for group
  //$insert = "INSERT INTO GroupNames (groupname) VALUES ('$groupID')";
  $createGroupTable = "CREATE TABLE $groupID (

    CreatorName varchar(50), userList varchar(20),
    user varchar(20), Clock time, Message varchar(255)
  )";
  $insertUserAdmin = "INSERT INTO $groupID (CreatorName, UserList) VALUES ("$clients[$ip][1]", "$clients[$ip][1]")";
  if ($groupID_exists > 0)
{
  $sendback = "FAIL\n";
  fwrite($sock, $sendback);
}
  else {
      mysqli_query($connection, $createGroupTable);
      mysqli_query($connection, $insertUserAdmin);
      $sendback = "SUCC";
      $message = "{$sendback}{$groupID} {$username}";
      echo "$message";
      $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT);
      fwrite($sock, "{$messageSize}{$message}");

    }
  if($connection->close()) {
    echo "Database Closed\n";
  }
}

function joinGroup($groupname, $username, $clients, $sock) {
  // Create connection
  $connection =  new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  // Check connection
  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);
  else
    echo "Connected to database \n";

  $check_groupname = "SELECT table_name FROM information_schema.tables WHERE TABLE_SCHEMA='StudyGroup' AND table_name='$groupname'";
  if ($stmt = mysqli_prepare($connection, $check_groupname)) {
    mysqli_stmt_execute($stmt);
    mysqli_stmt_store_result($stmt);
    $groupname_exists = mysqli_stmt_num_rows($stmt);
    mysqli_stmt_close($stmt);
  }

  $return_userList = "SELECT userList FROM $groupname";
  $join_group = "INSERT INTO $groupname (userList) VALUES ('$username')";
  if ($groupname_exists > 0) {
    $result = mysqli_query($connection, $return_userList);
    $row_count = $result->num_rows;
    if ($row_count < 4) {
      fwrite($sock, "SUCC\n");
      mysqli_query($connection, $join_group);
      $result2 = mysqli_query($connection, $return_userList);
      $num_user = $result2->num_rows;
      while($num_user > 0) {
        $row=mysqli_fetch_array($result2);
        fwrite($sock,"$row[0]\n");
        $num_user = $num_user - 1;
      }
    }
    else {
      fwrite($sock, "FAIL. Max Capacity \n");
    }
  }
  else {
    fwrite($sock, "FAIL\n");
  }

  if($connection->close()) {
    echo "Database Closed\n";
  }
}

?>
