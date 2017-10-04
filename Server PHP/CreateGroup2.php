<?php
// Create a new group given username input
include_once 'db_credentials.php';

function createGroup($groupname, $username, $sock)
{
  // Create connection
  $connection =  mysqli_connect(DB_Server, DB_User, DB_Pass, DB_Name);
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
    CreatorName varchar(50), GroupName varchar(50), userList varchar(20),
    user varchar(20), Clock time, Message varchar(255)
  )";
  $insertUserAdmin = "INSERT INTO $groupID (CreatorName, UserList) VALUES ('$username', '$username')";
  if ($groupID_exists > 0)
{
  $sendback = "Failure, groupID already exists. \n";
  fwrite($sock, $sendback);
}
  else {
      mysqli_query($connection, $createGroupTable);
      mysqli_query($connection, $insertUserAdmin);
      $sendback = "Success! Group created!\n";
      fwrite($sock, $sendback);
      fwrite($sock, "$groupID\n");
      fwrite($sock, "$username\n");

    }
  if($connection->close()) {
    echo "Database Closed\n";
  }
}

?>
