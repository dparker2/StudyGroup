<?php
//Functions for Account Creation 
//Create Account, LOGIN, LOGOUT.
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
    $sendback = "Failure, username already exists.\n";
    fwrite($sock, $sendback);
  }
  elseif ($email_exists > 0) {
    $sendback = "Failure, email already exists.\n";
    fwrite($sock, $sendback);
  }
  else {
    if (($result = mysqli_query($connection, $insert)) === TRUE){
      $sendback = "Success! Account created";
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

  // Check connection
    if ($connection->connect_error)
        die("Connection failed: " . $conn->connect_error);
    else
      echo "Connected to database \n";

  $check_password = "SELECT Pass FROM UserInfo WHERE Username = '$username'";
  $check_username = "SELECT Username FROM UserInfo WHERE Username = '$username'";
  //Checks if username exists before attempting to login, will return error otherwise.
  if ($result1 = mysqli_query($connection, $check_username)) {
    $obj = $result1->fetch_object();
    if ($obj->Username == $username) {
      if ($result = mysqli_query($connection, $check_password)) {
        $obj = $result->fetch_object();
        if ($obj->Pass == $password)
          fwrite($sock, "Login Successful!\n");
        else
          fwrite($sock, "Password Incorrect \n");
        mysqli_free_result($result);
      }
    }
    else
      fwrite($sock, "Username does not exist\n");
    mysqli_free_result($result1);
  }


  if ($connection->close()) {
    echo "Databased Closed\n";
  }
}




?>
