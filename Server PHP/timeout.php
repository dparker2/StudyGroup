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
	$time_start = microtime_float();
	static $t = $time_start; // need time_start to be static, cant directly declare it as static.
	$a++;
	if (a==3){ // 3 failed attempts
		$time_end = microtime_float();
		$time = $time_end - $t;
		if ($time > 60){ // 60 seconds since first fail, restart timeout
			$t = microtime_float();
			$a = 1;
		}
		else { // a warning for imminent timeout
			$message = "3 login fails";
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
			$message = "Account locked";
			sendMessage($message, $sock);
		}
	}
}

// with this login and securityQ share the same lockout. should they be separate?
function securityTimeout(){
	static $a = 0; // attempt counter
	$time_start = microtime_float();
	static $t = $time_start; // need time_start to be static, cant directly declare it as static.
	$a++;
	if (a==3){ // 3 failed attempts
		$time_end = microtime_float();
		$time = $time_end - $t;
		if ($time > 60){ // 60 seconds since first fail, restart timeout
			$t = microtime_float();
			$s = 1;
		}
		else { // a warning for imminent timeout
			$message = "3 security fails";
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
			$message = "Account locked";
			sendMessage($message, $sock);
		}
	}
}

?>
