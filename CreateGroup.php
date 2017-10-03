<?php
// Create a new group given username input

function createGroup($groupname, $sock)
{
  // Server info
  $servername = "csci150-mysql-sg.cvawt8ol1m2q.us-east-2.rds.amazonaws.com";
  $serverusername = "admin";
  $serverpassword = "csci1502017";
  $dbname = "StudyGroup";
  // Create connection
  $connection = new mysqli($servername, $serverusername, $serverpassword, $dbname);
  // Check connection
    if ($connection->connect_error){
      die("Connection failed: " . $connection->connect_error);
    else
      echo "Connected to database \n";
}
  // sql command to check if groupname already exists
  $check_groupname = "SELECT * FROM GroupNames WHERE groupname = '$groupname'";
  // Run, store, and close sql commands
  if ($stmt = mysqli_prepare($connection, $check_groupname))
{
    mysqli_stmt_execute($stmt);
    mysqli_stmt_store_result($stmt);
    $groupname_exists = mysqli_stmt_num_rows($stmt);
    mysqli_stmt_close($stmt);
}

  //if group name exists, return failure. else, run sql insert and return success
  $insert = "INSERT INTO GroupNames (groupname) VALUES ('$groupname')";
  if ($groupname_exists > 0)
{
  $sendback = "Failure, groupname already exists. \n";
  fwrite($sock, $sendback);
}
  else {
      mysqli_query($connection, $insert);
      $sendback = "Success! Group created!\n";
      fwrite($sock, $sendback);
    }
  if($connection->close()) {
    echo "Database Closed\n";
  }
}

?>
