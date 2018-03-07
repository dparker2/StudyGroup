<?php

include_once 'utilityFunctions.php';

// 2 global variabls for keeping track of failed attempts a= login, s=security
$a = 0;
$s = 0;

// returns the exact time, to the millisecond, this function was called. in UTC
function microtime_float() {
    list($usec, $sec) = explode(" ", microtime());
    return ((float)$usec + (float)$sec);
}

// i need to verify what type of time i'm getting so i can set the 2nd if statement'
function loginTimeout(){
	global $a;
	$time_start = microtime_float();
	static $t = $time_start; // need time_start to be static, cant directly declare it as static.
	$a++;
	if (a==4){ // 4 failed attempts
		$time_end = microtime_float();
		$time = $time_end - $t;
		if ($time > 60){ // 60 seconds since first fail, restart timeout
			$t = microtime_float();
			$a = 1;
		}
		else { // a warning for imminent timeout
			$message = "4 login fails";
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
		else { // still need some sort of punishment here. how to implement the lockout?
			$message = "5+ login fails";
			sendMessage($message, $sock);
		}
	}
}

function securityTimeout(){
	global $s;
	$time_start = microtime_float();
	static $t = $time_start;
	$s++;
	if (a==4){ // 4 failed attempts
		$time_end = microtime_float();
		$time = $time_end - $t;
		if ($time > 60){ // 60 seconds since first fail, restart timeout
			$t = microtime_float();
			$s = 1;
		}
		else {
			$message = "4 security fails";
			sendMessage($message, $sock);
		}
	}
	else if (a>=5){ // 5+ failed attempts
		$time_end = microtime_float();
		$time = $time_end - $t;
		if ($time > 60){ // 60 seconds since first fail, restart timeout
			$t = microtime_float();
			$s = 1;
		}
		else {
			$message = "5+ security fails";
			sendMessage($message, $sock);
		}
	}
}

?>
