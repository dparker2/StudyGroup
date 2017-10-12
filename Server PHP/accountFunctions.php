<?php
//Functions for Account Creation
//Create Account, LOGIN, LOGOUT.
include_once 'db_credentials.php';

function createAccount($email, $username, $password, $sock) {
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

  if ($username_exists > 0) { //returns failcase of username existing.
    $message = "FAILUsername exists, please try again.";
    echo "Debug: $message";
    $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT);
    fwrite($sock, "{$messageSize}{$message}");
  }
  elseif ($email_exists > 0) {//returns failcaise of email existing.
    $message = "FAILEmail exists, please try again.";
    echo "Debug: $message";
    $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT);
    fwrite($sock, "{$messageSize}{$message}");
  }
  else {
    if (($result = mysqli_query($connection, $insert)) === TRUE){
      $message = "SUCCSuccess! User Account created.";
      echo "Debug: $message";
      $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT);
      fwrite($sock, "{$messageSize}{$message}");
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
  if ($resultUser = mysqli_query($connection, $check_username)) {
    $obj = $resultUser->fetch_object(); //Returns result of username into object
    if ($obj->Username == $username) { //Accesses object and compares to username
      if ($resultPass = mysqli_query($connection, $check_password)) {
        $obj = $resultPass->fetch_object();
        if ($obj->Pass == $password){ //compares password to the one inputted
          $message = "00004SUCC"; //Successful if matches and writes back.
          fwrite($sock, $message);
          echo "Debugging: $message \n";
          $return_bool = true;
          mysqli_query($connection, $change_online);
        } //Closes password check.
        else{
          $message = "FAILPassword incorrect, please try again.";
          echo "Debug: $message";
          $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT);
          fwrite($sock, "{$messageSize}{$message}");
        }
        mysqli_free_result($resultPass);
      }//Closes Password Access
    } //Closes Username Check
    else{
      $message = "FAILUser does not exist, please try again.";
      echo "Debug: $message";
      $messageSize = str_pad((string)strlen($message), 5, "0", STR_PAD_LEFT);
      fwrite($sock, "{$messageSize}{$message}");
    }
    mysqli_free_result($resultUser);
  }//Closes Username Access

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
      fwrite($sock, "00004SUCC");
    }
    else {
      fwrite($sock, "00004FAIL\n");
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


// unfinised account recovery
function recoverAccount($email, $sock) {
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
      $newPass = //new pass from UI goes here
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

?>
