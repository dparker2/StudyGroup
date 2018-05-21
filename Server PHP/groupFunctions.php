<?php
include_once 'db_credentials.php';
include_once 'utilityFunctions.php';
include_once 'flashCardFunctions.php';
include_once 'classes.php';
date_default_timezone_set('UTC');

//CREATE GROUP FUNCTION
//Function creates group by comparing groupname to existing groups on database, if none found it will create a new group with the group name appended with a random 4 digits after, and sets the creator as Admin of the group
function createGroup($groupname, $user, $clientList, $sock)
{
  global $groupList;
  $connection = connectGroupDB();
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

  if (($groupIDExists = checkExistsDB($connection, $checkGroupID)) > 0) {
    $message = "FAILGroup already exists";
    sendMessage($message, $sock);
  }
  else {
      echo "DEBUG: Creating group $groupID ...\n";
      mysqli_query($connection, $createGroupTable);
      mysqli_query($connection, $insertUserAdmin);
      mysqli_query($connection, $insertUserList);
      mysqli_query($connection, $insertWBph);
      mysqli_query($connection, $createFlashCardTable);
      $groupList[$groupID] = new Group;
      $groupClass = $groupList["$groupID"];
      $groupClass->setGroupID($groupID);
      $groupClass->setAdmin($username);
      $groupClass->addMember($user);
      $user->setGroup($groupID);
      $user->setRecGroup($groupID);
      var_dump($groupList);
      $message = "SUCC"."$groupID";
      sendMessage($message, $sock);
      updateGroupList($connection, $clientList, $groupClass, $groupID);
      updateRecentGroups($user, $groupList);
    }
  disconnectDB($connection);
}

//JOIN GROUP FUNCTION
//Function inserts user into group table and updates chat, whiteboard, flashcards, and userlist.
function joinGroup($groupID, $user, $clientList, $sock)
{
  global $groupList;
  $connection = connectGroupDB();
  $username = $user->getName();
  $ip = $user->getIP();
  //SQL Statements to Query
  $checkGroupID = "SELECT table_name FROM information_schema.tables WHERE TABLE_SCHEMA='StudyGroup' AND table_name='$groupID'";
  $checkAdmin = "SELECT userList FROM $groupID WHERE (SELECT Admin FROM $groupID WHERE Admin IS NOT NULL) = userList";
  $selectAdmin = "SELECT Admin FROM $groupID WHERE Admin IS NOT NULL";
  $selectWBstring = "SELECT Whiteboard FROM $groupID WHERE Whiteboard IS NOT NULL";
  $selectExistingMember = "SELECT ipAddress FROM $groupID WHERE userList != '$username' LIMIT 1";

  if (($groupIDExists = checkExistsDB($connection, $checkGroupID)) > 0) {
    if (array_key_exists($groupID, $groupList)) {
      $groupClass = $groupList[$groupID];
      $numUsers = $groupClass->getNumMembers();
      $adminName = $groupClass->getAdmin();
    }
    else {
      $adminName = getObjStringDB($connection, $selectAdmin)->Admin;
      $groupList[$groupID] = new Group;
      $groupClass = $groupList[$groupID];
      $groupClass->setGroupID($groupID);
      $groupClass->setAdmin($adminName);
      $numUsers = $groupClass->getNumMembers();
    }

    echo "DEBUG: Admin of $groupID is $adminName\n";
    echo "DBUG: $username is joining group $groupID ... \n\n";
    echo "DEBUG: Number of users in group: $numUsers \n\n";
    if ($numUsers < 5) { //Max Group Size currently set to 5
      if(($numUsers == 4) && (($adminExists = checkExistsDB($connection, $checkAdmin)) < 1) && ($adminName != $username) ) //Checks to see if admin is in group before max capacity
        fwrite($sock, "00021FAILNo Admin in Group");
      else {
        fwrite($sock, "00004SUCC");
        $user->setGroup($groupID);
        $user->setRecGroup($groupID);
        $groupClass->addMember($user);
        updateGroupList($connection, $clientList, $groupClass, $groupID);
        updateGroupChat($connection, $groupID, $sock);
        updateFlashCards($connection, $ip, $clientList, $groupID, $sock);
        updateRecentGroups($user, $groupList);
        $favoriteGroups = $user->getFavoriteGroups();
        if (in_array($groupID, $favoriteGroups)) {
          $message = "NWFG"."$groupID";
          sendMessage($message, $sock);
        }
        echo "DEBUG: Determining whiteboard existence... \n";
        if ($numUsers == 0) {
          $object = getObjStringDB($connection, $selectWBstring);
          $wbstring = (string)$object->Whiteboard;
          if($wbstring == "" || $wbstring =="placeholder"){
            echo "DEBUG: No whiteboard in Database or placeholder, nothing to send...\n\n";
          }
          else {
            $message = "WBUP$groupID $wbstring";
            echo "DEBUG: Whiteboard found in Database, updating for first user...\n\n";
            sendMessage($message, $sock);
          }
        }
        else {
          echo "Requesting whiteboard from existing user to update joining member... \n\n";
          $existMembers = $groupClass->getMemberIP();
          $existingMember = $existMembers[0];
          $existingSocket = $clientList[$existingMember]->getSocket();
          $message = "NUWB$groupID $ip";
          sendMessage($message, $existingSocket);
        }
      }
    }
    else
      fwrite($sock, "00016FAILMax Capacity");
  }
  else
    fwrite($sock, "00023FAILGroup Doesn't Exist");
  disconnectDB($connection);
}

//LEAVE GROUP FUNCTION
//Deletes user from group table and updates rest of members with udpated user list.
function leaveGroup($groupID, $user, $clientList, $sock)
{
  global $groupList;
  $connection = connectGroupDB();
  $username = $user->getName();
  echo "DEBUG: $username Leaving Group... \n";
  fwrite($sock, "00004SUCC");
  $groupClass = $groupList[$groupID];
  $groupClass->removeMember($username);
  updateGroupList($connection, $clientList, $groupClass, $groupID);
  $user->removeCurrGroup($groupID);
  disconnectDB($connection);
}

//UPDATE GROUP LIST FUNCTION
//Updates group members of user list
function updateGroupList($connection, $clientList, $groupClass, $groupID)
{
  $groupID = $groupClass->getGroupID();
  $ipList = $groupClass->getMemberIP();
  $memberList = $groupClass->getMembers();
  foreach($ipList as $userIP) {
    $keySock = $clientList[$userIP]->getSocket();
    $message = "USCH"."$groupID";
    sendMessage($message, $keySock);
    foreach($memberList as $member) {
      $membername = $member->getName();
      $message = "NUSR"."$groupID $membername";
      sendMessage($message, $keySock);
    }
  }
}

//SEND CHAT FUNCTION
//Append username and timestamp to user message and returns to each member in group.
function sendChatMessage($groupID, $message, $user, $clientList, $sock)
{
  global $groupList;
  $connection = connectGroupDB();
  $escMessage = mysqli_escape_string($connection, $message);
  fwrite($sock, "00004SUCC");
  $username = $user->getName();
  $timestamp = date("Y-m-d H:i:s");
  $fullmessage = "$groupID $username $timestamp $message";
  echo "DEBUG: Sending Message... \n";
  echo "DEBUG: This is the full message $fullmessage \n";
  $ipList = $groupList[$groupID]->getMemberIP();
  foreach($ipList as $ip) {
    $socket = $clientList[$ip]->getSocket();
    $newMessage = "NCHT$fullmessage";
    sendMessage($newMessage,$socket);
  }
  $insertChat = "INSERT INTO $groupID (user, Clock, Message)
                VALUES ('$username', current_timestamp(), '$escMessage')";
  if (!mysqli_query($connection, $insertChat))
    die(mysqli_error($connection));
  disconnectDB($connection);
}

//UPDATE GROUP CHAT FUNCTION
//Gets group chat log from database and updates the socket calling it upon joining group
function updateGroupChat($connection, $groupID, $sock)
{
  $returnMessageList = "SELECT user, Clock, Message
                        FROM $groupID
                        WHERE Message IS NOT NULL";
  $messageList = mysqli_query($connection, $returnMessageList);
  echo "DEBUG: Updating Group Chat... \n\n";
  while($row = mysqli_fetch_array($messageList)) {
    $messages = "$groupID $row[0] $row[1] $row[2]";
    $message = "NCHT$messages";
    sendMessage($message, $sock);
  }
}

?>
