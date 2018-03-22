<?php

include_once 'utilityFunctions.php';

// returns the exact time, to the millisecond, this function was called. in UTC
function microtime_float() {
    list($usec, $sec) = explode(" ", microtime());
    return ((float)$usec + (float)$sec);
}

// i need to verify what type of time i'm getting so i can set the 2nd if statement'
function loginTimeout($username){
	static $a = 0; // attempt counter
	static $t = 0; // total time elapsed since first fail counter
	$check_lockout = "SELECT LockoutStatus FROM UserInfo WHERE Username = '$username'";
	if(($account_locked = checkExists($connection, $check_lockout)) > 0) {
		if ($time > 300) { // time since lockout > 5 mins
			unlockAccount($username); // unlock the account
		}
		else{
			$message = "FAILThis account is currently locked.";
			sendMessage($message, $sock);
			exit("account locked");
		}
	}
	$time_start = microtime_float();
	$t = $t + $time_start;
	$a++;
	if (a==3){ // 3 failed attempts
		$time_end = microtime_float();
		$time = $time_end - $t;
		if ($time > 60){ // 60 seconds since first fail, restart timeout
			$t = microtime_float();
			$a = 1;
		}
		else { // a warning for imminent timeout
			$message = "FAIL3";
			sendMessage($message, $sock);
		}
	}
	else if (a>=5){ // 5+ failed attempts
		$time_end = microtime_float();
		$time = $time_end - $t;
		if ($time > 60){ // 60 seconds since first fail, restart timeout
			$t = microtime_float();
			$a = 1;
		}
		else { // places a lock on the accounts ability to log in
			$change_lockout = "UPDATE UserInfo SET LockoutStatus='1' WHERE Username = '$username'";
			mysqli_query($connection, $change_lockout);
			$message = "FAILAccount now locked";
			sendMessage($message, $sock);
		}
	}
}

// with this login and securityQ share the same lockout. should they be separate for any reason?
// not sure where to call this yet \o/
function securityTimeout($username){
	static $a = 0; // attempt counter
	static $t = 0; // total time elapsed since first fail counter
	$check_lockout = "SELECT LockoutStatus FROM UserInfo WHERE Username = '$username'";
	if(($account_locked = checkExists($connection, $check_lockout)) > 0) {
		if ($time > 300) { // time since lockout > 5 mins
			unlockAccount($username); // unlock the account
		}
		else{
			$message = "FAILThis account is currently locked.";
			sendMessage($message, $sock);
			exit("account locked");
		}
	}
	$time_start = microtime_float();
	$t = $t + $time_start;
	$a++;
	if (a==3){ // 3 failed attempts
		$time_end = microtime_float();
		$time = $time_end - $t;
		if ($time > 60){ // 60 seconds since first fail, restart timeout
			$t = microtime_float();
			$a = 1;
		}
		else { // a warning for imminent timeout
			$message = "FAIL3";
			sendMessage($message, $sock);
		}
	}
	else if (a>=5){ // 5+ failed attempts
		$time_end = microtime_float();
		$time = $time_end - $t;
		if ($time > 60){ // 60 seconds since first fail, restart timeout
			$t = microtime_float();
			$a = 1;
		}
		else {// places a lock on the accounts ability to answer security questions
			$change_lockout = "UPDATE UserInfo SET LockoutStatus='1' WHERE Username = '$username'";
			mysqli_query($connection, $change_lockout);
			$message = "FAILAccount now locked";
			sendMessage($message, $sock);
		}
	}
}

// call to unlock an account and reset the fail counter + fail timer
function unlockAccount($username){
	$unlock_acc = "UPDATE LockoutStatus SET LockoutStatus='0' WHERE Username = '$username'"; // actualy unlocks the account
	mysqli_query($connection, $unlock_acc);
	$message = "SUCCAccount now unlocked";
	sendMessage();

	$t = microtime_float(); // might as well restart the timer
	$a = 0; // reset fail counter to 0
}
?>
