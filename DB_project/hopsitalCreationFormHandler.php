<?php 
$servername = "localhost";
$username = "root";
$password = "";
$db = "Hospital";
$conn = new mysqli($servername, $username, $password,$db);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
else{
	$hospitalName = mysqli_real_escape_string($conn,$_POST['HospitalName']);
	$adminName = mysqli_real_escape_string($conn,$_POST['RootAdmin']); 
	$username = mysqli_real_escape_string($conn,$_POST['RootAdminUsername']); 
	$pass =  mysqli_real_escape_string($conn,$_POST['Password']);  
	$hashedPass = password_hash($pass, PASSWORD_DEFAULT);

	$rootHospital = "INSERT INTO HospitalRoot(hospitalName,RootAdminName) VALUES('$hospitalName','$adminName')";
	if(!mysqli_query($conn, $rootHospital)){
		echo ("<script>console.log( 'Failed to create hospitalRoot ');</script>");
	}

	$rootAdmin = "INSERT INTO 
	Admins(
				AID,
				ASSN,
				AName,
				Username,
				Password,
				LastLogIn,
				isRootAdmin,
				isDatabaseAdmin,
				isEmployee
			)
	VALUES (
	01,
	NULL,
	'$adminName',
	'$username',
	'$hashedPass',
	 now(),
	1,
	1,
	1)";
	if(!mysqli_query($conn, $rootAdmin)){
		echo ("<script>console.log( 'Failed to create rootAdmin ');</script>");
	}
}
header( 'Location: index.html' ) ;
$conn->close();
?>