<?php
//Functions for Account Creation
//Create Account, LOGIN, LOGOUT, change password, recover account using email / recovery questions.
include_once 'db_credentials.php';

function createAccount($username, $password, $email, $sock) {
  // Create connection
  $connection = new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);

  // Check connection
    if ($connection->connect_error)
        die("Connection failed: " . $connection->connect_error);
    else
      echo "Connected to database \n";

  $check_username = "SELECT * FROM UserInfo WHERE Username = '$username'";
  $check_email = "SELECT * FROM UserInfo WHERE Email = '$email'";
  if ($stmt = mysqli_prepare($connection, $check_username)){
    //Execute query
    mysqli_stmt_execute($stmt);
    //Store result of query
    mysqli_stmt_store_result($stmt);

    $username_exists = mysqli_stmt_num_rows($stmt);

    //Close statement
    mysqli_stmt_close($stmt);
  }

  if ($stmt = mysqli_prepare($connection, $check_email)){
    //Execute Query
    mysqli_stmt_execute($stmt);
    //Store result of querymysqli_free_result($insert);
    mysqli_stmt_store_result($stmt);

    $email_exists = mysqli_stmt_num_rows($stmt);

    //Close Statement
    mysqli_stmt_close($stmt);
  }

  //Insert Query
  $insert = "INSERT INTO UserInfo (Username, Pass, Email) VALUES ('$username', '$password', '$email')";

  if ($username_exists > 0) {
    $sendback = "FAIL";
    fwrite($sock, $sendback);
  }
  elseif ($email_exists > 0) {
    $sendback = "FAIL";
    fwrite($sock, $sendback);
  }
  else {
    if (($result = mysqli_query($connection, $insert)) === TRUE){
      $sendback = "SUCC";
      fwrite($sock, $sendback);
      mysqli_free_result($result);
    }
  }
  if($connection->close()) {
    echo "Database Closed\n";
  }
}

function loginAccount($username, $password, $sock){
  // Create connection
  $connection = new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  $return_bool = false;

  // Check connection
    if ($connection->connect_error)
        die("Connection failed: " . $conn->connect_error);
    else
      echo "Connected to database \n";

  $check_password = "SELECT Pass FROM UserInfo WHERE Username = '$username'";
  $check_username = "SELECT Username FROM UserInfo WHERE Username = '$username'";
  $change_online = "UPDATE UserInfo SET Status='Online' WHERE Username = '$username'";
  //Checks if username exists before attempting to login, will return error otherwise.
  if ($result1 = mysqli_query($connection, $check_username)) {
    $obj = $result1->fetch_object();
    if ($obj->Username == $username) {
      if ($result = mysqli_query($connection, $check_password)) {
        $obj = $result->fetch_object();
        if ($obj->Pass == $password){
          $sendback = "SUCC";
          $message = "$sendback";
          //echo "$message";
          $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT);
          fwrite($sock, "{$messageSize}{$message}");
          echo "{$messageSize}{$message}";
          $return_bool = true;
          mysqli_query($connection, $change_online);
        }
        else
          fwrite($sock, "FAIL\n");
        mysqli_free_result($result);
      }
    }
    else
      fwrite($sock, "FAIL\n");
    mysqli_free_result($result1);
  }

  if ($connection->close()) {
    echo "Database Closed\n";
  }
  return $return_bool;
}

function logoutAccount($username, $sock) {
  $connection = new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  // Check connection
    if ($connection->connect_error)
        die("Connection failed: " . $conn->connect_error);
    else
      echo "Connected to database \n";

    $change_offline = "UPDATE UserInfo SET Status='Offline' WHERE Username = '$username'";
    if (mysqli_query($connection, $change_offline)) {
      fwrite($sock, "SUCC\n");
    }
    else {
      fwrite($sock, "FAIL\n");
    }
    if ($connection->close()){
      echo "Database Closed \n";
    }
}

//unfinished code to change a users password
function changePassword($username, $password, $sock) {
  $connection = new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  // Check connection
  if ($connection -> connect_error)
    die("connection failed: " . $conn->connect_error);
  else
    echo "Connected to database \n";

  // UI should now send a 'they did it' message and a new password
  $newPass = nul; //new pass from UI goes here
  $change_password = "UPDATE UserInfo SET Pass= 'newPass' WHERE Username = '$username'";
  if (mysqli_query($connection, $change_password)) {
    fwrite($sock, "SUCC\n");
  }
  else {
    fwrite($sock, "FAIL\n");
  }

  if ($connection->close()) {
    echo "Database Closed \n";
  }
}

// unfinised account recovery using email method
function recoverAccount($email, $password, $sock) {
  $connection = new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  // Check connection
  if ($connection -> connect_error)
    die("Connection failed: " . $conn->connect_error);
  else
    echo "Connected to database \n";

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

if ($connection->close()){
  echo "Database Closed \n";
  }
}

// account recovery using recovery question method.
// requires UserInfo table to be updated with Question column
// should recovery question itself be stored somewhere?
// if recovery questions are mandatory, this function can be included in account creation
function recoveryQset($username, $question, $sock) {
  $connection = new mysqli(DB_Server, DB_User, DB_Pass, DB_Name);
  // Check connection
  if ($connection -> connect_error)
    die("Connection failed: " . $conn->connect_error);
  else
    echo "Connected to database \n";
  
  //store recovery question answer into the table
  $setAnswer = "UPDATE UserInfo set Question = 'question' WHERE Username = 'username'";
  if (mysqli_query($connection, $setAnswer)) {
    fwrite($sock, "SUCC\n");
  }
  else {
    fwrite($sock, "FAIL\n");
  }

  if ($connection->close()) {
    echo "Database Closed \n";
  }
?>
