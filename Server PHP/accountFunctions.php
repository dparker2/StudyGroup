<?php
include_once 'db_credentials.php';
include_once 'sendEmail.php';
include_once 'utilityFunctions.php';
include_once 'classes.php';

function createAccount($email, $username, $password, $sock) {
  $connection = connectAccountDB();

  $check_username = "SELECT * FROM UserInfo WHERE Username = '$username'";
  $check_email = "SELECT * FROM UserInfo WHERE Email = '$email'";

  //Insert Query
  $insert = "INSERT INTO UserInfo (Username, Pass, Email) VALUES ('$username', '$password', '$email')";

  if (($username_exists = checkExistsDB($connection, $check_username)) > 0) {
    $message = "FAILUsername exists, please try again.";
    sendMessage($message, $sock);
  }
  elseif (($email_exists = checkExistsDB($connection, $check_email)) > 0) {
    $message = "FAILEmail exists, please try again.";
    sendMessage($message, $sock);
  }
  else {
    if ($result = mysqli_query($connection, $insert)){
      $message = "SUCCSuccess! User Account created.";
      sendMessage($message, $sock);
      sendRegEmail($email);
    }
  }
  disconnectDB($connection);
}

function loginAccount($username, $password, $client, $sock) {
  global $groupList;
  $connection = connectAccountDB();
  $check_password = "SELECT Pass FROM UserInfo WHERE Username = '$username'";
  $check_username = "SELECT Username FROM UserInfo WHERE Username = '$username'";
  $check_email = "SELECT Email FROM UserInfo WHERE Username = '$username'";
  $change_online = "UPDATE UserInfo SET UserStatus='Online' WHERE Username = '$username'";
  $check_recent_groups = "SELECT RecentGroups FROM UserInfo WHERE Username = '$username' AND RecentGroups IS NOT NULL";
  $check_favorite_groups = "SELECT FavoriteGroups FROM UserInfo WHERE Username = '$username' AND FavoriteGroups IS NOT NULL";

  if (($username_exists = checkExistsDB($connection, $check_username)) > 0) {
      $checkPass = getObjStringDB($connection, $check_password)->Pass;
      if ($checkPass == $password) {
        $resultEmail = getObjStringDB($connection, $check_email)->Email;
        $message = "SUCC{$resultEmail}";
        sendMessage($message, $sock);
        mysqli_query($connection, $change_online);
        $client->setName($username);
        $client->setEmail($resultEmail);
        if (checkExistsDB($connection, $check_favorite_groups) > 0) {
          $favorite_groups = getObjStringDB($connection, $check_favorite_groups)->FavoriteGroups;
          $fav_group_array = array_reverse(explode(" ", $favorite_groups));
          if($fav_group_array != "" && $fav_group_array != " ") {
            foreach($fav_group_array as $group) {
              $client->setFavGroup($group);
            }
          }
        }
        if (checkExistsDB($connection, $check_recent_groups) > 0) {
          $recent_groups = getObjStringDB($connection, $check_recent_groups)->RecentGroups;
          $rec_group_array = array_reverse(explode(" ", $recent_groups));
          if($rec_group_array != "" && $rec_group_array != " ") {
            foreach($rec_group_array as $group) {
              $client->setRecGroup($group);
            }
          }
        }
        updateFavoriteGroups($client, $groupList);
        updateRecentGroups($client, $groupList);
      } 
      else{
        $message = "FAILPassword incorrect, please try again.";
        sendMessage($message, $sock);
      }
  }
  else{
    $message = "FAILUser does not exist, please try again.";
    sendMessage($message, $sock);
  }
  disconnectDB($connection);
}

function logout($user) {
  $accountDB = connectAccountDB();
  $groupDB = connectGroupDB();
  if ($user->getName() != NULL) {
    setOfflineDB($user, $accountDB);
    setFavoriteGroupsDB($user, $groupDB);
    setRecentGroupsDB($user, $groupDB);
    if (count($user->getCurrGroups()) != 0)
      clearFromGroupsGL($user, $groupDB);
    setOfflineCL($user);
  }
  disconnectDB($accountDB);
  disconnectDB($groupDB);
}
//unfinished code to change a users password, need client input
function changePassword($username, $password, $sock) {
  $connection = connectAccountDB();

  // UI should now send a 'they did it' message and a new password
  $newPass = nul; //new pass from UI goes here
  $change_password = "UPDATE UserInfo SET Pass= 'newPass' WHERE Username = '$username'";
  if (mysqli_query($connection, $change_password)) {
    fwrite($sock, "SUCC\n");
  }
  else {
    fwrite($sock, "FAIL\n");
  }

  disconnectDB($connection);
}

// unfinised account recovery using email method. outdated, unused, unloved
function recoverAccount($email, $password, $sock) {
  $connection = connectAccountDB();

  //check if email exists before attempting to send recovery email, will return error otherwise.
  $check_email = "SELECT Email FROM UserInfo WHERE Email = '$email'";
  $change_password = "UPDATE UserInfo SET Pass= '$newPass' WHERE Email = '$email'";
  if ($result1 = mysqli_query($connection, $check_email)) {
    $obj = $result1->fetch_object();
    if ($obj->Email == $email) {
      fwrite($sock, "SUCC\n");
      // create random id to be sent via email, save it in db
      $rID = rand(10000000, 99999999);
      $insertRecoveryTable = "INSERT INTO AccountRecovery (email, rID) VALUES ('$email', '$rID')";

      // UI should now send a 'they did it' message and a new password
      $newPass = '$password';//new pass from UI goes here
      mysqli_query($connection, $change_password);
    }
    else
      fwrite($sock, "FAIL\n");

    mysqli_free_result($result1);
  }

  $disconnectDB($connection);
}

// takes users email, returns users username.
function rememberUsername ($email, $sock) {
  $connection = connectAccountDB();

  $find_user = "SELECT Username FROM UserInfo WHERE Email = '$email'";  //finds a username tied to a email
  $resultUser = mysqli_query($connection, $find_user); //runs find_user
  $obj = $resultUser->fetch_object();
  $returnUser = $obj->Username; // returnUser == return value of find_user
  $message = "SUCC{$returnUser}";
  sendMessage($message, $socket);

  disconnectDB($connection);
}

// recovery option for remembering a password, sends a recovery email
function rememberPassword ($username, $email, $sock) {
  $connection = connectAccountDB();

  $find_pass = "SELECT Pass FROM UserInfo WHERE (Email = '$email' AND Username = '$username')";
  $resultPass = mysqli_query($connection, $find_pass); //runs find_pass
  $obj = $resultPass->fetch_object();
  $returnPass = $obj->Pass; // returnUser == return value of find_Pass
  $message = "SUCC";
  recPWEmail($email, $username, $returnPass);
  sendMessage($message, $socket);

  disconnectDB($connection);
}

?>
