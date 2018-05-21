<?php


function sendRegEmail($email){
 $to = "$email";
 $subject = "Welcome to StudyGroup!";
 $body = "Hi,\n\nHow are you? \nThis is a confirmation email letting you know that your account $email has been created.";
 if (mail($to, $subject, $body)) {
 echo("<p>Email successfully sent!</p>");
 } else {
 echo("<p>Email delivery failed…</p>");
 }
}

function recPWEmail($email, $user, $pass){
  $headers  = 'MIME-Version: 1.0' . "\r\n";
  $headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
  $to = "$email";
  $subject = "Password Recovery!";
  $body = "<html><body>";
  $body .= "<h1> Hi, How are you?<br>You recently asked to recover your password.<br>The password for $user is $pass.<br><br>";
  $body .= "<img src='http://ec2-18-221-67-202.us-east-2.compute.amazonaws.com/success.jpg' style='width:600px;height:500px;' /></h1>"; //success kid
  $body .= "</body></html>";
  if (mail($to, $subject, $body, $headers)) {
  echo("<p>Email successfully sent!</p>");
  } else {
  echo("<p>Email delivery failed…</p>");
  }



}


?>
