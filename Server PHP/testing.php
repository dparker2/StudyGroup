<?php

include_once 'db_credentials.php';
include_once 'utilityFunctions.php';
include_once 'groupFunctions.php';
include_once 'sendEmail.php';
//include_once 'whiteboardFunctions.php';


/*
$connection = connectGroup();
$message = "hello world";
echo "this is the message: $message\n\n";
$escMessage = mysqli_real_escape_string($connection, $message);
echo "this is the escape string: $escMessage\n\n";
$unescMessage = stripslashes($escMessage);
echo "this is the unescaped string: $unescMessage\n\n";
*/

/*
$message = "green car";
echo "$message\n\n";
$newMessage = str_replace(' ', '_', $message);
echo "this is the message after str_replace: $newMessage\n\n";
*/

/*
$connection = connectAccount();
$testUsers = array("Juan", "Garret", "Fuller", "Michelle", "David", "Liu", "Ruby", "Dhanyu", "Cecotti");
for ($i = 0; $i < 2000; $i++) {
  //echo $testUsers[array_rand($testUsers)];
  $user = $testUsers[array_rand($testUsers)] . rand(1000, 9999);
  $insert = "INSERT INTO UserInfoTest (Username) VALUES ('$user')";
  $check_username = "SELECT * FROM UserInfoTest WHERE Username = '$user'";
  if (($username_exists = checkExists($connection, $check_username)) >  0) {
    echo "Username exists already!: $user\n\n";
    }
  else {
    mysqli_query($connection, $insert);
  }
  }


disconnect($connection);
*/
$email = "mejia.juan.dev@gmail.com";
$user = "testemail";
sendRecCode($email, $user);



?>
