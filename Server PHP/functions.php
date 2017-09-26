<?php
//Functions for server
//CREATE Function, JOIN Function, etc.

function createAccount($username, $password, $email, $sock) {
  $servername = "csci150-mysql-sg.cvawt8ol1m2q.us-east-2.rds.amazonaws.com";
  $serverusername = "admin";
  $serverpassword = "csci1502017";
  $dbname = "StudyGroup";
  // Create connection
  $connection = new mysqli($servername, $serverusername, $serverpassword, $dbname);

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
  $servername = "csci150-mysql-sg.cvawt8ol1m2q.us-east-2.rds.amazonaws.com";
  $serverusername = "admin";
  $serverpassword = "csci1502017";
  $dbname = "StudyGroup";
  // Create connection
  $connection = new mysqli($servername, $serverusername, $serverpassword, $dbname);

  // Check connection
    if ($connection->connect_error)
        die("Connection failed: " . $conn->connect_error);
    else
      echo "Connected to database \n";

  $check_password = "SELECT Pass FROM UserInfo WHERE Username = '$username'";
  if ($result = mysqli_query($connection, $check_password)) {
    $obj = $result->fetch_object();
    if ($obj->Pass == $password)
      fwrite($sock, "Login Successful!\n");
    else
      fwrite($sock, "Password Incorrect \n");
    mysqli_free_result($result);
  }

  if ($connection->close()) {
    echo "Databased Closed\n";
  }
}




?>
