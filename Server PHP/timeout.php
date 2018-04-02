<?php
function confirmIPAddress($value) {

  // LoginAttempts table. looks at # of attempts, and time >=5 minutes. used to create an array for later.
  // may split this into multiple variables for better readability
  $q = "SELECT Attempts, (CASE when LastLogin is not NULL and DATE_ADD(LastLogin, INTERVAL ".TIME_PERIOD." 
  MINUTE)>NOW() then 1 else 0 end) as Denied FROM LoginAttempts WHERE IP = '$value'"; 

  $result = mysql_query($q, $this->connection); 
  $data = mysql_fetch_array($result); 

  //Verify that at least one login attempt is in database 

  if (!$data) { // redundancy, should never happen
    return 0; 
  } 
  if ($data["Attempts"] >= 5) { // if user has failed >= 5 times
    if($data["Denied"] == 1) { // and time < 5 minutes
      return 1; // ip is locked out
    } 
    else { // time > 5 minutes, reset attempts
      $this->clearLoginAttempts($value); 
      return 0; 
    } 
  } 
  return 0; 
} 

function addLoginAttempt($value) {

   //Increase number of attempts. Set last login attempt if required.
   $q = "SELECT * FROM LoginAttempts WHERE IP = '$value'"; 
   $result = mysql_query($q, $this->connection);
   $data = mysql_fetch_array($result);
   
   if($data) { // if not first time attempting to log in
     $attempts = $data["attempts"]+1; 
     // $attempts = "UPDATE LoginAttempts SET Attempts = Attempts + 1 WHERE IP = $value";      

     if($attempts==5) { // just hit 5 fails, update time and attempt# in db for comnfirmIPAddress
       $q = "UPDATE LoginAttempts SET Attempts='$attempts', LastLogin=NOW() WHERE ip = '$value'";
       $result = mysql_query($q, $this->connection);
     }
     else { // not yet at 5, just update db
       $q = "UPDATE LoginAttempts SET Attempts='$attempts' WHERE ip = '$value'";
       $result = mysql_query($q, $this->connection);
     }
   }
   else { // first attempt from this ip, create a database entry
     $q = "INSERT INTO LoginAttempts (Attempts,IP,LastLogin) values (1, '$value', NOW())";
     $result = mysql_query($q, $this->connection);
   }
}

function clearLoginAttempts($value) { // set # of attempts from $value to 0
  $q = "UPDATE LoginAttempts SET Attempts = 0 WHERE ip = '$value'"; 
  return mysql_query($q, $this->connection);
}

?>

