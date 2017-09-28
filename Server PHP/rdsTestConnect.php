<?php
$servername = "csci150-mysql-sg.cvawt8ol1m2q.us-east-2.rds.amazonaws.com";
$username = "admin";
$password = "csci1502017";

// Create connection
$conn = new mysqli($servername, $username, $password);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
echo "Connected successfully\n";

?>
