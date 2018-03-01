<?php

include_once 'utilityFunctions.php';

$a = 0;
$s = 0;

function microtime_float() {
    list($usec, $sec) = explode(" ", microtime());
    return ((float)$usec + (float)$sec);
}

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
		else {
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
		else {
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
