<?php
include_once 'db_credentials.php';
include 'utilityFunctions.php';
date_default_timezone_set('UTC');
//CREATE GROUP FUNCTION
function createGroup($groupname, $ip, $clients, $sock)
{
  // Create connection
  $connection =  new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  // Check connection
  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);

  $rand4 = rand(1000, 9999); //Random 4 digits for end of group ID
  $groupID = $groupname . "_" . $rand4; //Creates random groupname + 4 random digits.
  // SQL command to check if groupID table already exists
  $check_groupID = "SELECT table_name FROM information_schema.tables WHERE TABLE_SCHEMA='StudyGroup' AND ta ble_name='$groupID'";
  //if group name exists, return failure. else, run query and create table for group
  $createGroupTable = "CREATE TABLE $groupID (
    Admin varchar(50), userList varchar(20), ipAddress varchar(50),
    user varchar(20), Clock time, Message varchar(255)
  )";
  $username = $clients[$ip][1]; //Takes username from dict. $clients.
  $insertUserAdmin = "INSERT INTO $groupID (Admin) VALUES ('$username')";
  $insertUserList = "INSERT INTO $groupID (UserList, ipAddress) VALUES ('$username', '$ip')";
  if (($groupID_exists = checkExists($connection, $check_groupID)) > 0) {
    $sendback = "00024FAILGroup already exists";  //Sends back fail if group already exists.
    fwrite($sock, $sendback);
  }
  else { // Runs queries and creates group, adding the current user as the user admin/creator
      mysqli_query($connection, $createGroupTable);
      mysqli_query($connection, $insertUserAdmin);
      mysqli_query($connection, $insertUserList);
      $sendback = "SUCC";
      $message = "{$sendback}{$groupID}";
      echo "$message";
      $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT);
      fwrite($clients[$ip][0], "{$messageSize}{$message}");

    }
  $connection->close();
}

//JOIN GROUP FUNCTION
function joinGroup($groupID, $ip, $clients, $sock) {
  // Create connection
  $connection =  new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  // Check connection
  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);

  //Checks group name if it exists in table.  Stores result in groupname_exists
  $check_groupID = "SELECT table_name FROM information_schema.tables WHERE TABLE_SCHEMA='StudyGroup' AND table_name='$groupID'";
  $username = $clients[$ip][1]; //Using client dict. to store username
  //SQL Commands
  $return_userList = "SELECT userList FROM $groupID WHERE userList IS NOT NULL";
  $join_group = "INSERT INTO $groupID (userList, ipAddress) VALUES ('$username', '$ip')";
  $checkAdmin = "SELECT userList FROM $groupID WHERE (SELECT Admin FROM $groupID WHERE Admin IS NOT NULL) = userList";
  $selectAdmin = "SELECT Admin FROM $groupID WHERE Admin IS NOT NULL";

  if (($groupID_exists = checkExists($connection, $check_groupID)) > 0) {
    $result = mysqli_query($connection, $return_userList);
    $row_count = $result->num_rows;
    //Returns current admin into adminname as string to compare
    if ($resultAdmin = mysqli_query($connection, $selectAdmin)) {
      $obj = $resultAdmin->fetch_object();
      $adminname = $obj->Admin;
    }

    if ($row_count < 5) { //Max group size currently set at 5.
      if(($row_count == 4) && (($adminExists = checkExists($connection, $checkAdmin)) < 1) && ($adminname != $username) ) //Checks to see if admin is in group before max capacity
        fwrite($sock, "00021FAILNo Admin in Group");
      else {
        fwrite($sock, "00004SUCC");
        mysqli_query($connection, $join_group); //Inserts username and ip to list.
        updateGroupList($connection, $ip, $clients, $groupID, $sock);
        updateGroupChat($connection, $ip, $clients, $groupID, $sock); //Writes back current list to each ip
      }
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

//LEAVE GROUP FUNCTION
function leaveGroup($groupID, $ip, $clients, $sock) {
  $connection = new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);

  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);
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

//UPDATE GROUP LIST FUNCTION
function updateGroupList($connection, $ip, $clients, $groupID, $sock) {
  //SQL Commands
  $return_userList = "SELECT userList FROM $groupID WHERE userList IS NOT NULL";
  $return_ipList = "SELECT ipAddress FROM $groupID WHERE ipAddress IS NOT NULL";
  $resultIP = mysqli_query($connection, $return_ipList); //Returns list of current IP addresses i.e. current user list connected.
  $num_ip = $resultIP->num_rows; //Stores number of people currently connected for while loop iteration.


  while($num_ip > 0) { //Loops through each active client, printing out the current user list in order to update ui
    $resultUsers = mysqli_query($connection, $return_userList); //runs and stores results of all usernames in the group currently
    $num_user = $resultUsers->num_rows;//stores number of usernames
    $rowIP = mysqli_fetch_array($resultIP); //Fetches first IP as an array.
    //echo "Debugging: This is keyIP we're using to index: $rowIP[0] \n";
    $keyIP = $rowIP[0]; //Stores the key IP address:Port for use of $clients dict.
    $keySock = $clients[$keyIP][0]; //Uses the above to access the socket client of the IP address to write back to.
    //echo "Debugging: This is keySock we're writing to: $keySock \n";
    fwrite($keySock, "00004USCH"); //Notifies client that wave of new users will be updated.

    for($n_user = $num_user; $n_user > 0; $n_user = $n_user - 1){ //For loop that iterates through lists of username to writeback to client.
      $row=mysqli_fetch_array($resultUsers); //Fetches first username into array
      $name = $row[0]; //Stores name into variable
      //echo "Debugging: We are writing User: $row[0] to $keyIP with socket $keySock \n";
      $message = "NUSR$name"; //Appends CODE NUSR to username
      $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT); //Pads left of code with length of string so client knows how much to read
      fwrite($keySock,"{$messageSize}{$message}"); //Writes back to client.
      echo "Debugging: Client should be receiving: {$messageSize}{$message} \n";
    } //closes for loop

    $num_ip = $num_ip - 1; //Goes to next IP address/User in group
  }//end while Loops
}

function sendChatMessage($groupID, $message, $ip, $clients, $sock) {
  // Create connection
  $connection =  new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  // Check connection
  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);

  $escMessage = mysqli_escape_string($message); //escape handles single quotes and such.
  fwrite($sock, "00004SUCC");
  $username = $clients[$ip][1];
  $timestamp = date("h:i:s");
  $fullmessage = "$username $timestamp $message";
  echo "DEBUG: This is the full message $message \n";
  $return_ipList = "SELECT ipAddress FROM $groupID WHERE ipAddress IS NOT NULL";
  $resultIP = mysqli_query($connection, $return_ipList); //Returns list of current IP addresses i.e. current user list connected.
  $num_ip = $resultIP->num_rows; //Stores number of people currently connected for while loop iteration.
  while($num_ip > 0) { //Loops through each active client, printing out the current user list in order to update ui
    //Entire section below is to keep track of sockets we're writing to.
    $rowIP = mysqli_fetch_array($resultIP); //Fetches first IP as an array.
    //echo "Debugging: This is keyIP we're using to index: $rowIP[0] \n";
    $keyIP = $rowIP[0]; //Stores the key IP address:Port for use of $clients dict.
    $keySock = $clients[$keyIP][0]; //Uses the above to access the socket client of the IP address to write back to.
    //echo "Debugging: This is keySock we're writing to: $keySock \n";
    fwrite($keySock, "00004UCHT"); //Notifies client that wave of new users will be updated.
    //$messages = "$row[0] $row[1] $row[2]"; //Stores user clock message into variable
    //echo "Debugging: We are writing $fullmessage to $keyIP with socket $keySock \n";
    $message1 = "NCHT$fullmessage"; //Appends CODE NCHT to chat message
    $messageSize = str_pad((string)strlen($message1), 5, "0", STR_PAD_LEFT); //Pads left of code with length of string so client knows how much to read
    fwrite($keySock,"{$messageSize}{$message1}"); //Writes back to client.
    //echo "Debugging: Client should be receiving: {$messageSize}{$message1} \n";

    $num_ip = $num_ip - 1; //Goes to next IP address/User in group
  }//end while Loops
  //SQL Commands
  //echo "DEBUG: $username and $message being put into $groupID \n";
  $insertChat = "INSERT INTO $groupID (user, Clock, Message)
                VALUES ('$username', current_timestamp(), '$escMessage')";
  if (!mysqli_query($connection, $insertChat)) {
    die(mysqli_error($connection));
  }
  if($connection->close()) {
    echo "Database closed\n";
  }
}

function updateGroupChat($connection, $ip, $clients, $groupID, $sock) {
  $connection =  new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  // Check connection
  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);
  //SQL Commands
  $return_Messages = "SELECT user, Clock, Message
                        FROM $groupID
                        WHERE Message IS NOT NULL";
  $resultMessages = mysqli_query($connection, $return_Messages); //runs and stores results of all messages in the group currently.
  $num_Messages = $resultMessages->num_rows;//stores number of messages
  //echo "We are writing to current user joining group: $ip who's socket should be $sock \n";
  fwrite($sock, "00004UCHT"); //Notifies Client that wave of chat is going to be returned

  //echo "Debugging: What is the number of messages: $num_Messages \n";
  for($n_messages = $num_Messages; $n_messages > 0; $n_messages = $n_messages - 1){ //For loop that iterates through lists of messages to writeback to client.
    $row=mysqli_fetch_array($resultMessages); //Fetches first message into array
    $messages = "$row[0] $row[1] $row[2]"; //Stores user clock message into variable
    //echo "Debugging: We are writing $messages to ip $ip with socket $sock \n";
    $message = "NCHT$messages"; //Appends CODE NUSR to username
    $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT); //Pads left of code with length of string so client knows how much to read
    fwrite($sock,"{$messageSize}{$message}"); //Writes back to client.
    //echo "Debugging: Client should be receiving: {$messageSize}{$message} \n";
  } //closes for loop

}

function updateWhiteBoard($groupID, $point1, $point2, $ip, $clients, $sock) {
  $connection =  new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  // Check connection
  if ($connection->connect_error)
    die("Connection failed: " . $connection->connect_error);

  $return_ipList = "SELECT ipAddress FROM $groupID WHERE ipAddress IS NOT NULL";
  $resultIP = mysqli_query($connection, $return_ipList); //Returns list of current IP addresses i.e. current user list connected.
  $num_ip = $resultIP->num_rows; //Stores number of people currently connected for while loop iteration.
  while($num_ip > 0) { //Loops through each active client, printing out the current user list in order to update ui
    //Entire section below is to keep track of sockets we're writing to.
    $rowIP = mysqli_fetch_array($resultIP); //Fetches first IP as an array.
    //echo "Debugging: This is keyIP we're using to index: $rowIP[0] \n";
    $keyIP = $rowIP[0]; //Stores the key IP address:Port for use of $clients dict.
    $keySock = $clients[$keyIP][0]; //Uses the above to access the socket client of the IP address to write back to.
    if ($keySock != $sock) { // Makes sure doesn't write back to original socket.
      //echo "Debugging: This is keySock we're writing to: $keySock \n";
      $message = "WBLN$point1 $point2"; 
      $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT); //Pads left of code  with length of string so client knows how much to read
      fwrite($keySock,"{$messageSize}{$message}"); //Writes back to client.
      //echo "Debugging: Client should be receiving: {$messageSize}{$message} \n";
    }
    $num_ip = $num_ip - 1; //Goes to next IP address/User in group
  }//end while Loops
  if($connection->close()) {
    echo "Database closed\n";
  }
}
?>
