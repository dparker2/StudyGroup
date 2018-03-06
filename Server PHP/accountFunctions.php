<?php
include_once 'db_credentials.php';
include_once 'sendEmail.php';
include_once 'utilityFunctions.php';
include_once 'classes.php';
/* Group Functions
  function createAccount($email, $username, $password, $sock);
  function loginAccount($username, $password, $sock);
  function logoutAccount($username, $sock);
  function changePassword($username, $password, $sock);
  function recoverAccount($email, $password, $sock);
  function rememberUsername ($email, $sock);
  function rememberPassword ($username, $email, $sock);
*/

function createAccount($email, $username, $password, $sock) {
  $connection = connectAccount();

  $check_username = "SELECT * FROM UserInfo WHERE Username = '$username'";
  $check_email = "SELECT * FROM UserInfo WHERE Email = '$email'";

  //Insert Query
  $insert = "INSERT INTO UserInfo (Username, Pass, Email) VALUES ('$username', '$password', '$email')";

  if (($username_exists = checkExists($connection, $check_username)) > 0) { //returns failcase of username existing.
    $message = "FAILUsername exists, please try again.";
    sendMessage($message, $sock);
  }
  elseif (($email_exists = checkExists($connection, $check_email)) > 0) {//returns failcaise of email existing.
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
  disconnect($connection);
}

function loginAccount($username, $password, $sock) {
  $connection = connectAccount();
  $bool_check = false;
  $check_password = "SELECT Pass FROM UserInfo WHERE Username = '$username'";
  $check_username = "SELECT Username FROM UserInfo WHERE Username = '$username'";
  $check_email = "SELECT Email FROM UserInfo WHERE Username = '$username'";
  $change_online = "UPDATE UserInfo SET UserStatus='Online' WHERE Username = '$username'";
  //Checks if username exists before attempting to login, will return error otherwise.
  if (($username_exists = checkExists($connection, $check_username)) > 0) {
      $checkPass = getObjString($connection, $check_password)->Pass;
      if ($checkPass == $password) {
        $resultEmail = getObjString($connection, $check_email)->Email;
        $message = "SUCC{$resultEmail}"; //Successful if matches and writes back email belonging to user for UI
        sendMessage($message, $sock);
        mysqli_query($connection, $change_online);
        $bool_check = true;
      } //Closes password check.
      else{
        $message = "FAILPassword incorrect, please try again.";
        sendMessage($message, $sock);
      }
  }
  else{
    $message = "FAILUser does not exist, please try again.";
    sendMessage($message, $sock);
  }
  disconnect($connection);
  return $bool_check;
}

function logoutAccount($username, $sock) {
  $connection = connectAccount();

  $change_offline = "UPDATE UserInfo SET UserStatus='Offline' WHERE Username = '$username'";
  if (mysqli_query($connection, $change_offline)) {
	fwrite($sock, "00004SUCC");
  }
  else {
    fwrite($sock, "00004FAIL\n");
  }
  disconnect($connection);

}

//unfinished code to change a users password, need client input
function changePassword($username, $password, $sock) {
  $connection = connectAccount();

  // UI should now send a 'they did it' message and a new password
  $newPass = nul; //new pass from UI goes here
  $change_password = "UPDATE UserInfo SET Pass= 'newPass' WHERE Username = '$username'";
  if (mysqli_query($connection, $change_password)) {
    fwrite($sock, "SUCC\n");
  }
  else {
    fwrite($sock, "FAIL\n");
  }

  disconnect($connection);
}

// unfinised account recovery using email method. outdated, unused, unloved
function recoverAccount($email, $password, $sock) {
  $connection = connectAccount();

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

  $disconnect($connection);
}

// takes users email, returns users username.
function rememberUsername ($email, $sock) {
  $connection = connectAccount();

  $find_user = "SELECT Username FROM UserInfo WHERE Email = '$email'";  //finds a username tied to a email
  $resultUser = mysqli_query($connection, $find_user); //runs find_user
  $obj = $resultUser->fetch_object();
  $returnUser = $obj->Username; // returnUser == return value of find_user
  $message = "SUCC{$returnUser}";
  sendMessage($message, $socket);

  disconnect($connection);
}

// recovery option for remembering a password, sends a recovery email
function rememberPassword ($username, $email, $sock) {
  $connection = connectAccount();

  $find_pass = "SELECT Pass FROM UserInfo WHERE (Email = '$email' AND Username = '$username')";
  $resultPass = mysqli_query($connection, $find_pass); //runs find_pass
  $obj = $resultPass->fetch_object();
  $returnPass = $obj->Pass; // returnUser == return value of find_Pass
  $message = "SUCC";
  recPWEmail($email, $username, $returnPass);
  sendMessage($message, $socket);

  disconnect($connection);
}

?>
