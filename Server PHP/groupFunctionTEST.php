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
  $groupID = $groupname . "_" . $rand4; //Creates random groupname + 4 random digits.
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
  $createGroupTable = "CREATE TABLE $groupID (
    CreatorName varchar(50), userList varchar(20), ipAddress varchar(50),
    user varchar(20), Clock time, Message varchar(255)
  )";
  $username = $clients[$ip][1]; //Takes username from dict. $clients.
  $insertUserAdmin = "INSERT INTO $groupID (CreatorName, UserList, ipAddress) VALUES ('$username', '$username', '$ip')";
  if ($groupID_exists > 0) {
    $sendback = "00024FAILGroup already exists";  //Sends back fail if group already exists.
    fwrite($sock, $sendback);
  }
  else { // Runs queries and creates group, adding the current user as the user admin/creator
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

function joinGroup($groupID, $ip, $clients, $sock) {
  // Create connection
  $connection =  new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  // Check connection
  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);
  else
    echo "Connected to database \n";

  //Checks group name if it exists in table.  Stores result in groupname_exists
  $check_groupID = "SELECT table_name FROM information_schema.tables WHERE TABLE_SCHEMA='StudyGroup' AND table_name='$groupID'";
  if ($stmt = mysqli_prepare($connection, $check_groupID)) {
    mysqli_stmt_execute($stmt);
    mysqli_stmt_store_result($stmt);
    $groupID_exists = mysqli_stmt_num_rows($stmt);
    mysqli_stmt_close($stmt);
  }

  $username = $clients[$ip][1]; //Using client dict. to store username
  //SQL Commands
  $return_userList = "SELECT userList FROM $groupID";
  $join_group = "INSERT INTO $groupID (userList, ipAddress) VALUES ('$username', '$ip')";

  if ($groupID_exists > 0) {
    $result = mysqli_query($connection, $return_userList);
    $row_count = $result->num_rows;

    if ($row_count < 4) { //Max group size currently set at 4.
      fwrite($sock, "00004SUCC");
      mysqli_query($connection, $join_group); //Inserts username and ip to list.
      updateGroupList($connection, $ip, $clients, $groupID, $sock); //Writes back current list to each ip
    }//closes if statement for capacity
    else {
      fwrite($sock, "00016FAILMax Capacity"); //Fail case for max capcity.
    }
  }//closes if statement for group exists
  else {
    fwrite($sock, "00023FAILGroup Doesn't Exist"); //Fail case that groupname doesn't exist
  }

  if($connection->close()) {
    echo "Database Closed\n";
  }
}

function leaveGroup($groupID, $ip, $clients, $sock) {
  $connection = new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);

  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);
  else
    echo "Connected to database \n";
  //SQL Commands
  $username = $clients[$ip][1];
  $leaveGroup = "DELETE FROM $groupID
    WHERE userList = '$username'";
  $return_userList = "SELECT userList FROM $groupID";
  //Delete from database.
  mysqli_query($connection, $leaveGroup);
  fwrite($sock, "00004SUCC"); //writes back to current client success on leave
  updateGroupList($connection, $ip, $clients, $groupID, $sock);
  if($connection->close()) {
    echo "Database closed\n";
  }
}



function updateGroupList($connection, $ip, $clients, $groupID, $sock) {
  $return_userList = "SELECT userList FROM $groupID";
  $return_ipList = "SELECT ipAddress FROM $groupID";

  $resultIP = mysqli_query($connection, $return_ipList); //Returns list of current IP addresses i.e. current user list connected.
  $num_ip = $resultIP->num_rows; //Stores number of people currently connected for while loop iteration.


  while($num_ip > 0) { //Loops through each active client, printing out the current user list in order to update ui
    $resultUsers = mysqli_query($connection, $return_userList); //runs and stores results of all usernames in the group currently
    $num_user = $resultUsers->num_rows;//stores number of usernames
    $rowIP = mysqli_fetch_array($resultIP); //Fetches first IP as an array.
    echo "Debugging: This is keyIP we're using to index: $rowIP[0] \n";
    $keyIP = $rowIP[0]; //Stores the key IP address:Port for use of $clients dict.
    $keySock = $clients[$keyIP][0]; //Uses the above to access the socket client of the IP address to write back to.
    echo "Debugging: This is keySock we're writing to: $keySock \n";
    fwrite($keySock, "00004USCH"); //Notifies client that wave of new users will be updated.

    for($n_user = $num_user; $n_user > 0; $n_user = $n_user - 1){ //For loop that iterates through lists of username to writeback to client.
      $row=mysqli_fetch_array($resultUsers); //Fetches first username into array
      $name = $row[0]; //Stores name into variable
      echo "Debugging: We are writing $row[0] to $keyIP with socket $keySock \n";
      $message = "NUSR$name"; //Appends CODE NUSR to username
      $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT); //Pads left of code with length of string so client knows how much to read
      fwrite($keySock,"{$messageSize}{$message}"); //Writes back to client.
      echo "Debugging: Client should be receiving: {$messageSize}{$message} \n";
    } //closes for loop

    $num_ip = $num_ip - 1; //Goes to next IP address/User in group
  }//end while Loops
}



?>
