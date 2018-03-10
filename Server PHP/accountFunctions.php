<?php
include_once 'db_credentials.php';
include_once 'sendEmail.php';
include_once 'utilityFunctions.php';
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

  $passwordHash = password_hash($password, PASSWORD_DEFAULT);

  $defaultQ1 = "What_is_your_mother's_maiden_name?";
  $defaultQ2 = "What_is_the_name_of_the_street_you_grew_up_on?";
  $defaultQ3 = "What_was_the_name_of_your_first_pet?";

  $defaultQ1 = mysqli_real_escape_string($connection, $defaultQ1);
  $defaultQ2 = mysqli_real_escape_string($connection, $defaultQ2);
  $defaultQ3 = mysqli_real_escape_string($connection, $defaultQ3);

  //Insert Query
  $insert = "INSERT INTO UserInfo (Username, Pass, Email, SQ1, SQ2, SQ3) VALUES ('$username', '$passwordHash', '$email', '$defaultQ1', '$defaultQ2', '$defaultQ3')";

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
    else {
      echo("Error description: " . mysqli_error($connection));
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
      if (password_verify($password, $checkPass)) {
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
  $query = "UPDATE UserInfo SET Pass='$password' WHERE Username = '$username'";
  if (!mysqli_query($connection, $query)) {
    $message = "FAILSomething went wrong. Either the username does not exist or there was an issue connecting to the database.\n\n";
    sendMessage($message, $sock);
  }
  else {
    $message = "SUCCPassword was successfully changed!\n\n";
    sendMessage($message, $sock);
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
  if(checkExists($connection, $find_user) > 0) {
    $returnUser = getObjString($connection, $find_user)->Username;
//    $message = "SUCC1Email has been sent with your username. Make sure to check your spam folder!\n";
    $message = "RUSRSUCC Email has been sent with your username. Make sure to check your spam folder!\n";
    sendMessage($message, $sock);
    sendVerEmail($email, $returnUser);
  }
  else {
    $message = "FAILNo username found with that email. Try again!\n";
    sendMessage($message, $sock);
  }
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
