<?php
include_once 'db_credentials.php';
include_once 'utilityFunctions.php';
include_once 'flashCardFunctions.php';
date_default_timezone_set('UTC');
/* Group Functions
  function createGroup($groupname, $user, $sock)
  function joinGroup($groupID, $user, $clientList, $sock)
  function leaveGroup($groupID, $user, $clientList, $sock)
  function updateGroupList($connection, $clientList, $groupID)
  function sendChatMessage($groupID, $message, $user, $clientList, $sock)
  function updateGroupChat($connection, $groupID, $sock) */

//CREATE GROUP FUNCTION
//Function creates group by comparing groupname to existing groups on database, if none found it will create a new group with the group name appended with a random 4 digits after, and sets the creator as Admin of the group
function createGroup($groupname, $user, $clientList, $sock)
{
  $connection = connectGroup();
  $username = $user->getName();
  $ip = $user->getIP();
  $rand4 = rand(1000, 9999);
  $groupID = $groupname . "_" . $rand4;
  $flashGroupID = "$groupID" . "FC";

  //SQL Statements to Query
  $checkGroupID = "SELECT table_name FROM information_schema.tables WHERE TABLE_SCHEMA='StudyGroup' AND table_name='$groupID'";
  $createGroupTable = "CREATE TABLE $groupID (
    Admin varchar(50), userList varchar(20), ipAddress varchar(50),
    User varchar(20), Clock datetime, Message varchar(255), Whiteboard LONGTEXT)";
  $insertUserAdmin = "INSERT INTO $groupID (Admin) VALUES ('$username')";
  $insertUserList = "INSERT INTO $groupID (UserList, ipAddress) VALUES ('$username', '$ip')";
  $insertWBph = "INSERT INTO $groupID (Whiteboard) VALUES ('placeholder')";
  $createFlashCardTable = "CREATE TABLE $flashGroupID (
    id INT NOT NULL AUTO_INCREMENT, user varchar(20), side1 varchar(255) , side2 varchar(255) , primary key (id))";

  //Returns Fail message if group already exits.
  if (($groupIDExists = checkExists($connection, $checkGroupID)) > 0) {
    $message = "FAILGroup already exists";
    sendMessage($message, $sock);
  }
  //Else Creates group, makes creator admin, inserts it into user list, puts a blank placeholder for whiteboard, and creates the flash card table. Returns SUCC message to client.
  else {
      echo "DEBUG: Creating group $groupID ...\n";
      mysqli_query($connection, $createGroupTable);
      mysqli_query($connection, $insertUserAdmin);
      mysqli_query($connection, $insertUserList);
      mysqli_query($connection, $insertWBph);
      mysqli_query($connection, $createFlashCardTable);
      $message = "SUCC"."$groupID";
      sendMessage($message, $sock);
      updateGroupList($connection, $clientList, $groupID);
    }
  disconnect($connection);
}

//JOIN GROUP FUNCTION
//Function inserts user into group table and updates chat, whiteboard, flashcards, and userlist.
function joinGroup($groupID, $user, $clientList, $sock)
{
  $connection = connectGroup();        //Connect to group database.
  $username = $user->getName();   //Gets username from object and assigns to username
  $ip = $user->getIP();           //Get ip from object and assigns to ip
  $bool_check = false;

  //SQL Statements to Query
  $checkGroupID = "SELECT table_name FROM information_schema.tables WHERE TABLE_SCHEMA='StudyGroup' AND table_name='$groupID'";
  $returnUserList = "SELECT userList FROM $groupID WHERE userList IS NOT NULL";
  $joinGroup = "INSERT INTO $groupID (userList, ipAddress) VALUES ('$username', '$ip')";
  $checkAdmin = "SELECT userList FROM $groupID WHERE (SELECT Admin FROM $groupID WHERE Admin IS NOT NULL) = userList";
  $selectAdmin = "SELECT Admin FROM $groupID WHERE Admin IS NOT NULL";
  $selectWBstring = "SELECT Whiteboard FROM $groupID WHERE Whiteboard IS NOT NULL";
  $selectExistingMember = "SELECT ipAddress FROM $groupID WHERE userList != '$username' LIMIT 1";

  /*
  Checks if group that user entered exists.
  Returns Fail message if the group doesn't exists, or if the current user is the last person to join the group and the administrator hasn't joined yet.
  Else, it will insert the user into the group, then update the group list, flash cards, group chat, then whitebaord.*/
  if (($groupIDExists = checkExists($connection, $checkGroupID)) > 0) { // True if group does exist
    $numUsers = getNumRows($connection, $returnUserList);        //Obtains curr number of users in group from database
    $adminName = getObjString($connection, $selectAdmin)->Admin; //Obtains admin name for comparison
    echo "DEBUG: Admin of $groupID is $adminName\n";
    echo "DBUG: $username is joining group $groupID ... \n";
    echo "DEBUG: Number of users in group: $numUsers \n";
    if ($numUsers < 5) { //Max Group Size currently set to 5
      if(($numUsers == 4) && (($adminExists = checkExists($connection, $checkAdmin)) < 1) && ($adminName != $username) ) //Checks to see if admin is in group before max capacity
        fwrite($sock, "00021FAILNo Admin in Group");
        //If able to join, will insert user to group table and update client for group list, group chat, flash card, wb.
      else {
        fwrite($sock, "00004SUCC");
        mysqli_query($connection, $joinGroup);
        updateGroupList($connection, $clientList, $groupID);
        updateGroupChat($connection, $groupID, $sock);
        updateFlashCards($connection, $ip, $clientList, $groupID, $sock);

        echo "DEBUG: Determining whiteboard existence... \n";
        //If this is the first user joining
        if ($numUsers == 0) {
          $object = getObjString($connection, $selectWBstring);
          $wbstring = (string)$object->Whiteboard;
          //If WB is just empty or placeholder, nothing to update. Else send whiteboard to member
          if($wbstring == "" || $wbstring =="placeholder"){
            echo "DEBUG: No whiteboard in Database or placeholder, nothing to send...\n";
            $bool_check = true;
          }
          else {
            $message = "WBUP$groupID $wbstring";
            echo "DEBUG: Whiteboard found in Database, updating for first user...\n";
            sendMessage($message, $sock);
            $bool_check = true;
          } // closes line 89 else statement
        }   // closes line 82 numUsers if statement
        //Else not first user joining, will get current whiteboard from existing member and send to member joining
        else {
          echo "Requesting whiteboard from existing user to update joining member... \n";
          $object = getObjString($connection, $selectExistingMember);
          $existingMember = (string)$object->ipAddress;
          $existingSocket = $clientList[$existingMember]->getSocket();
          $message = "NUWB$groupID $ip";
          sendMessage($message, $existingSocket);
          $bool_check = true;
        } //closes line 95 else statement
      }   //closes line 74 else statement
    }     //closes line 71 if statement for capacity
    else
      fwrite($sock, "00016FAILMax Capacity");      //Fail case for max capcity.
  }       //closes line 66 if statement for group exists
  else
    fwrite($sock, "00023FAILGroup Doesn't Exist"); //Fail case that groupname doesn't exist
  disconnect($connection);
}

//LEAVE GROUP FUNCTION
//Deletes user from group table and updates rest of members with udpated user list.
function leaveGroup($groupID, $user, $clientList, $sock)
{
  $connection = connectGroup();
  $username = $user->getName();
  $leaveGroup = "DELETE FROM $groupID
    WHERE userList = '$username'";
  echo "DEBUG: $username Leaving Group... \n";
  mysqli_query($connection, $leaveGroup);
  fwrite($sock, "00004SUCC");             //writes back to current client success on leave
  updateGroupList($connection, $clientList, $groupID);
  disconnect($connection);
}

//UPDATE GROUP LIST FUNCTION
//Updates group members of user list
function updateGroupList($connection, $clientList, $groupID)
{
  //SQL Statements to Query
  /*$returnUserList = "SELECT userList FROM $groupID, UserInfo WHERE userList IS NOT NULL
  and userList = UserInfo.Username
  and UserInfo.Status = 'Online';";*/
  $returnUserList = "SELECT userList FROM $groupID WHERE userList IS NOT NULL";
  $returnIPList = "SELECT ipAddress FROM $groupID WHERE ipAddress IS NOT NULL";
  $ipList = mysqli_query($connection, $returnIPList); //Returns list of current IP

  echo "DEBUG: Updating Group List... \n";
  while($rowIP = mysqli_fetch_array($ipList)) {  //Loops through each member one at a time.
    $keyIP = $rowIP[0];                          //Stores one IP from current group list
    $keySock = $clientList[$keyIP]->getSocket(); //Stores socket to write to.
    $message = "USCH"."$groupID";
    sendMessage($message, $keySock);
             //Notifies client that wave of new users will be updated.
    $resultUsers = mysqli_query($connection, $returnUserList); //Obtains userList to write to member
    while($row=mysqli_fetch_array($resultUsers)) {             //Loops through each user and writes to member above.
      $name = $row[0];                                         //Stores one member to $name
      $message = "NUSR"."$groupID $name";                                  //Appends CODE NUSR to username so client can update.
      sendMessage($message, $keySock);
    } //closes for/while loop
  }   //end while Loops
}
//SEND CHAT FUNCTION
//Append username and timestamp to user message and returns to each member in group.
function sendChatMessage($groupID, $message, $user, $clientList, $sock)
{
  $connection = connectGroup();
  $escMessage = mysqli_escape_string($connection, $message);
  fwrite($sock, "00004SUCC");
  $username = $user->getName();
  $timestamp = date("Y-m-d H:i:s");
  $fullmessage = "$groupID $username $timestamp $message";      //Full message that include username timestamp and the message
  echo "DEBUG: Sending Message... \n";
  echo "DEBUG: This is the full message $fullmessage \n";

  $return_ipList = "SELECT ipAddress FROM $groupID WHERE ipAddress IS NOT NULL";
  $ipList = mysqli_query($connection, $return_ipList); //Returns ip list of current members
  while($rowIP = mysqli_fetch_array($ipList)) {        //Iterates through members and sends message
    $keyIP = $rowIP[0];                                //Gets IP to obtain socket
    $keySock = $clientList[$keyIP]->getSocket();       //Uses socket of existing member to send message
    $newMessage = "NCHT$fullmessage";                  //Appends CODE NCHT to chat message for client use
    sendMessage($newMessage, $keySock);
  }//end while Loops

  //Inserts chat into database for logging
  $insertChat = "INSERT INTO $groupID (user, Clock, Message)
                VALUES ('$username', current_timestamp(), '$escMessage')";
  if (!mysqli_query($connection, $insertChat))
    die(mysqli_error($connection));
  disconnect($connection);
}

//UPDATE GROUP CHAT FUNCTION
//Gets group chat log from database and updates the socket calling it upon joining group
function updateGroupChat($connection, $groupID, $sock)
{
  $returnMessageList = "SELECT user, Clock, Message
                        FROM $groupID
                        WHERE Message IS NOT NULL";
  $messageList = mysqli_query($connection, $returnMessageList);
  echo "DEBUG: Updating Group Chat... \n";

  //Sends chat log to requested socket
  while($row = mysqli_fetch_array($messageList)) {
    $messages = "$groupID $row[0] $row[1] $row[2]"; //Stores user: clock: message into variable
    $message = "NCHT$messages";            //Appends CODE NCHT for client use
    sendMessage($message, $sock);
  } //closes while loop
}

?>
