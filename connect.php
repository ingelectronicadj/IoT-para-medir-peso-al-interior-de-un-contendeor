<?php
//include 'connect.php'; 

$servername = "localhost";
$username = "root";
$password = "B0g0t42015*+";
$dbname = "treschepes";

// Create connection
$conn = new mysqli($servername, $username, $password,$dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
echo "Connected successfully";



?>


