<?php
//Functions for server
//CREATE Function, JOIN Function, etc.

function createAccount($username, $password, $email, $connection, $sock) {
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
    //Store result of query
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
    if ($connection->query($insert) === TRUE){
      $sendback = "Success! Account created";
      fwrite($sock, $sendback);
    }
  }
}

function loginAccount($username, $password, $connection, $sock){
  $check_password = "SELECT Pass FROM UserInfo WHERE Username = '$username'";
  if ($result = $connection->query($check_password)) {
    $obj = $result->fetch_object();
    if ($obj->Pass == $password)
      fwrite($sock, "Login Successful!\n");
    else
      fwrite($sock, "Password Incorrect \n");
  }
}




?>
