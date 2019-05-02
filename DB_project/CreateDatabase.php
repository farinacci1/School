<?php
$servername = "localhost";
$username = "root";
$password = "";

// Create connection
$conn = new mysqli($servername, $username, $password);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

//check if databse exist if not create it
if ($conn->select_db("Hospital") === FALSE) {
	$sql = "CREATE DATABASE Hospital";
	if ($conn->query($sql) === TRUE) {
		echo("<script>console.log('Database created successfully');</script>");
		
	} else {
		 echo ("<script>console.log( 'Error creating database: " . $conn->error . "' );</script>");
	}
	
}
else {
	echo("<script>console.log('Hospital Database already exists');</script>");
}
$conn->close();
?>