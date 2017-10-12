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

    CreatorName varchar(50), userList varchar(20), ipAddress varchar(50),
    user varchar(20), Clock time, Message varchar(255)
  )";
  $username = $clients[$ip][1];
  $insertUserAdmin = "INSERT INTO $groupID (CreatorName, UserList, ipAddress) VALUES ('$username', '$username', '$ip')";
  if ($groupID_exists > 0)
{
  $sendback = "FAIL\n";
  fwrite($sock, $sendback);
}
  else {
      mysqli_query($connection, $createGroupTable);
      mysqli_query($connection, $insertUserAdmin);
      $sendback = "SUCC";
      $message = "{$sendback}{$groupID}";
      echo "$message";
      $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT);
      fwrite($clients[$ip][0], "{$messageSize}{$message}");

    }
  if($connection->close()) {
    echo "Database Closed\n";
  }
}

function joinGroup($groupname, $ip, $clients, $sock) {
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

  $username = $clients[$ip][1];
  $return_userList = "SELECT userList FROM $groupname";
  $return_ipList = "SELECT ipAddress FROM $groupname";
  $join_group = "INSERT INTO $groupname (userList, ipAddress) VALUES ('$username', '$ip')";
  if ($groupname_exists > 0) {
    $result = mysqli_query($connection, $return_userList);
    $row_count = $result->num_rows;
    if ($row_count < 4) {
      mysqli_query($connection, $join_group);
      //$resultUsers = mysqli_query($connection, $return_userList);
      //$num_user = $resultUsers->num_rows;
      $resultIP = mysqli_query($connection, $return_ipList);
      $num_ip = $resultIP->num_rows;
      while($num_ip > 0) {
        $resultUsers = mysqli_query($connection, $return_userList);
        $num_user = $resultUsers->num_rows;
        $rowIP = mysqli_fetch_array($resultIP);
        echo "Debugging: This is keyIP we're using to index: $rowIP[0] \n";
        $keyIP = $rowIP[0];
        $keySock = $clients[$keyIP][0];
        echo "Debugging: This is keySock we're writing to: $keySock \n";
        fwrite($keySock, "00004USCH");
        for($n_user = $num_user; $n_user > 0; $n_user = $n_user - 1){
          $row=mysqli_fetch_array($resultUsers);
          $name = $row[0];
          echo "Debugging: We are writing $row[0] to $keyIP with socket $keySock \n";
          $message = "NUSR$name";
          $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT);
          fwrite($keySock,"{$messageSize}{$message}");
          echo "Client should be receiving: {$messageSize}{$message} \n";
        }
        $num_ip = $num_ip - 1;
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
