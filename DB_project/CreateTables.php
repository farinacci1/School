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
		$hospitalTable = "CREATE TABLE IF NOT EXISTS HospitalRoot (
			hospitalName VARCHAR(15) NOT NULL,
			RootAdminName VARCHAR(24) NOT NULL
		)";
		if(!mysqli_query($conn, $hospitalTable)){
			 echo ("<script>console.log( 'Error creating Table HospitalRoot: " . $conn->error . "' );</script>");
		}
		$AdminsTable = "CREATE TABLE IF NOT EXISTS Admins(
			AID INT PRIMARY KEY NOT NULL UNIQUE,
			ASSN INT UNIQUE,
			AName varchar(38) NOT NULL,
			Username VARCHAR(26) NOT NULL UNIQUE,
			Password VARCHAR(255) NOT NULL,
			LastLogIn DATETIME,
			isRootAdmin BOOLEAN,
			isAdmin BOOLEAN,
			isEmployee BOOLEAN
	    )";
		if(!mysqli_query($conn, $AdminsTable)){
			 echo ("<script>console.log( 'Error creating Table Admins: " . $conn->error . "' );</script>");
		}
		$DoctorsTable = "CREATE TABLE IF NOT EXISTS Doctors(
			DID INT PRIMARY KEY NOT NULL UNIQUE,
			DSSN INT NOT NULL UNIQUE,
			Dname VARCHAR(38) NOT NULL,
			Username VARCHAR(26) NOT NULL UNIQUE,
			Password VARCHAR(255) NOT NULL,
			numRatings INT,
			sumRatings INT,
			LastLogIn DATETIME,
			isEmployee BOOLEAN
		)";
		if(!mysqli_query($conn, $DoctorsTable)){
			 echo ("<script>console.log( 'Error creating Table Doctors: " . $conn->error . "' );</script>");
		}
		$PatientsTable = "CREATE TABLE IF NOT EXISTS Patients(
			PID INT PRIMARY KEY NOT NULL UNIQUE,
			PSSN INT NOT NULL UNIQUE,
			PName VARCHAR(38) NOT NULL,
			Username VARCHAR(26) NOT NULL UNIQUE,
			Password VARCHAR(255) NOT NULL,
			DOB DATE NOT NULL,
			LastLogIn DATETIME,
			Role VARCHAR(10),
			isPatient BOOLEAN
		)";
		if(!mysqli_query($conn, $PatientsTable)){
			 echo ("<script>console.log( 'Error creating Table Patients: " . $conn->error . "' );</script>");
		}

		$AppointmentsTable = "CREATE TABLE IF NOT EXISTS Appointments(
			AppId INT AUTO_INCREMENT PRIMARY KEY,
			APID INT NOT NULL,
			ADID INT NOT NULL,
			AppDate DATE NOT NULL,
			AppStartTime TIME NOT NULL,
			AppEndTime TIME NOT NULL,
			AppReason VARCHAR(128) NOT NULL,
			AppStats VARCHAR(20) NOT NULL,
			CHECK(AppStatus in ('pending','booked','canceled','kept','late','missed')),
			FOREIGN KEY(APID) REFERENCES Patients(PID),
			FOREIGN KEY(ADID) REFERENCES Doctors(DID)
		)";
		if(!mysqli_query($conn, $AppointmentsTable)){
			 echo ("<script>console.log( 'Error creating Table Appointments: " . $conn->error . "' );</script>");
		}

		$ScheduledSurgeries = "CREATE TABLE IF NOT EXISTS Surgeries(
			SID INT AUTO_INCREMENT PRIMARY KEY,
			SDID INT NOT NULL,
			SPID INT NOT NULL,
			SurgeryDate DATE NOT NULL,
			SurgeryStartTime TIME NOT NULL,
			SurgeryEndTime TIME NOT NULL,
			SurgeryType VARCHAR(128) NOT NULL,
			SurgeryRoom VARCHAR(32) NOT NULL,
			FOREIGN KEY(SDID) REFERENCES Doctors(DID),
			FOREIGN KEY(SPID) REFERENCES Patients(PID)
		)";
		if(!mysqli_query($conn, $ScheduledSurgeries)){
			 echo ("<script>console.log( 'Error creating Table Surgeries: " . $conn->error . "' );</script>");
		}
		$DoctorShifts = "CREATE TABLE IF NOT EXISTS Schedule(
			SDID INT NOT NULL,
			ShiftDate DATE NOT NULL,
			ShiftStartTime TIME NOT NULL,
			ShiftEndTime TIME NOT NULL,
			FOREIGN KEY (SDID) REFERENCES Doctors(DID)
		)";
		if(!mysqli_query($conn, $DoctorShifts)){
			echo ("<script>console.log( 'Error creating Table Schedule: " . $conn->error . "' );</script>");
		}
		$canPerformSugery = "CREATE TABLE IF NOT EXISTS Surgery(
			SDID INT NOT NULL,
			SurgeryType VARCHAR(128) NOT NULL,
			FOREIGN KEY(SDID) REFERENCES Doctors(DID)
		)";
		if(!mysqli_query($conn, $canPerformSugery)){
			 echo ("<script>console.log( 'Error creating Table Surgery: " . $conn->error . "' );</script>");
		}

		$PatientMedicalRecord = "CREATE TABLE IF NOT EXISTS MedicalRecord(
			MPID INT NOT NULL,
			heightCM int,
			weightKG int,
			age int NOT NULL,
			BloodType VARCHAR(3),
			CHECK(BloodType IN ('A+','A-','B+','B-','AB+','AB-','O+','O-','UNK')),
			FOREIGN KEY (MPID) REFERENCES Patients(PID)
		)";
		if(!mysqli_query($conn, $PatientMedicalRecord)){
			 echo ("<script>console.log( 'Error creating Table MedicalRecord: " . $conn->error . "' );</script>");
		}

		$HospitalTools = "CREATE TABLE IF NOT EXISTS Tools(
			ToolName VARCHAR(32) NOT NULL,
			ToolId INT NOT NULL UNIQUE
		)";
		if(!mysqli_query($conn, $HospitalTools)){
			echo ("<script>console.log( 'Error creating Table Tools: " . $conn->error . "' );</script>");
		}

		$ToolsInUse = "CREATE TABLE IF NOT EXISTS ToolsInUse(
			surgeryId INT NOT NULL,
			ToolId int,
			ToolStatus VARCHAR(15) NOT NULL,
			CHECK(ToolStatus IN ('checked out','checked in', 'discarded')),
			FOREIGN KEY (surgeryId) REFERENCES Surgeries(SID),
			FOREIGN KEY (ToolId) REFERENCES Tools(ToolId)
		)";
		if(!mysqli_query($conn, $ToolsInUse)){
			 echo ("<script>console.log( 'Error creating Table ToolsInUse: " . $conn->error . "' );</script>");
		}

}
$conn->close();
?>