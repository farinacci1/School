<?php
$servername = "localhost";
$username = "username";
$password = "password";

// Create connection
$conn = new mysqli($servername, $username, $password);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
//Select database Hospital
$mysqli->select_db("Hospital");
//If database could not be selected create database
if (!$db_selected) {
	$sql = "CREATE DATABASE Hospital";
	if ($conn->query($sql) === TRUE) {
		echo "Database created successfully";
	} else {
		echo "Error creating database: " . $conn->error;
	}
}
$conn->close();
?>