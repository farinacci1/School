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

session_start();
$_SESSION['selectednavBarOption'] ="";
if($_SERVER["REQUEST_METHOD"] == "POST") {
	$Username =  mysqli_real_escape_string($conn ,$_POST['Username']); 
	$Pass =  mysqli_real_escape_string($conn ,$_POST['Password']);
	$searchAdmin = "SELECT AName,Username,Password FROM Admins WHERE Username = '$Username'";
	$searchDoctor = "SELECT Dname,Username,Password  FROM Doctors WHERE Username = '$Username'";
	$searchPatients ="SELECT PName,Username,Password  FROM Patients WHERE Username = '$Username'";
	//Search Admin table
	$result = mysqli_query($conn,$searchAdmin);
	$row = mysqli_fetch_array($result,MYSQLI_ASSOC);
	$count = mysqli_num_rows($result);
	if ($count == 0){
		$UserNotFoundMsg = "<script>console.log( 'User not found');</script>";
	}
	elseif ($count == 1) {
		echo ("<script>console.log( 'Username exists in database');</script>");
		if(password_verify($Pass, $row["Password"])) {
			$_SESSION['login_username'] = $Username;
			$_SESSION['name'] = $row["AName"];
			$_SESSION['tableFoundIn'] = "Admin";
			$updateLogIn = "UPDATE Admins SET LastLogIn = now() WHERE Username = '$Username' ";
			$loginUpdatedResult = mysqli_query($conn,$updateLogIn);
			if($loginUpdatedResult){
				echo ("<script>console.log( 'User Login time has been update');</script>");
			}   
			if(isset($_SESSION['login_username'])) {
				header("Location: accountHome.html");
				exit;
			}
		}
		else{
			echo ("<script>console.log( 'Password entered is invalid');</script>");
			header("Location: index.html");
		}		
	}
	else{
		//Search doctor table
		$result = mysqli_query($conn,$searchDoctor);
		$row = mysqli_fetch_array($result,MYSQLI_ASSOC);
		$count = mysqli_num_rows($result);
		if($count == 1) {
			echo ("<script>console.log( 'Username exists in database');</script>");
			if(password_verify($Pass, $row["Password"])) {
				$_SESSION['login_username'] = $Username;
				$_SESSION['name'] = $row["Dname"];
				$_SESSION['tableFoundIn'] = "Doctor";
				$updateLogIn = "UPDATE Doctors SET LastLogIn = now() WHERE Username = $Username ";
				$loginUpdatedResult = mysqli_query($conn,$updateLogIn);
				if($loginUpdatedResult){
					echo ("<script>console.log( 'User Login time has been update');</script>");
				}
				if(isset($_SESSION['login_username'])) {
					header("Location: accountHome.html");
					exit;
				}				
			}
			else{
				echo ("<script>console.log( 'Password entered is invalid');</script>");
				header("Location: index.html");
			}		
		}
		else{
			//search patients table
			$result = mysqli_query($conn,$searchPatients);
			$row = mysqli_fetch_array($result,MYSQLI_ASSOC);
			$count = mysqli_num_rows($result);
			if($count == 1) {
				echo ("<script>console.log( 'Username exists in database');</script>");
				if(password_verify($Pass, $row["Password"])) {
					$_SESSION['login_username'] = $Username;
					$_SESSION['name'] = $row["PName"];
					$_SESSION['tableFoundIn'] = "Patient";
					$updateLogIn = "UPDATE Patients SET LastLogIn = now() WHERE Username = $Username ";
					$loginUpdatedResult = mysqli_query($conn,$updateLogIn);
					if($loginUpdatedResult){
						echo ("<script>console.log( 'User Login time has been update');</script>");
					}
					if(isset($_SESSION['login_username'])) {
						header("Location: accountHome.html");
						exit;
					}					
				}
				else{
					echo ("<script>console.log( 'Password entered is invalid');</script>");
					header("Location: index.html");
				}					

			}
			else{
				echo $UserNotFoundMsg;
				header("Location: index.html");
			}
		}
	}
}
$conn->close();
?>