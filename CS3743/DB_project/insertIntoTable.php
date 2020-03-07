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
function decodeAdminJSON($filePath){
	global $conn;
	$FileContents = file_get_contents($filePath);
	$array = json_decode($FileContents, true);
	
	foreach($array["Admins"] as $row){
		$adminId =  $row['AID'];
		$adminSSN = $row['ASSN'];
		$adminName = $row['AName'];
		$adminUser = $row['Username'];
		$adminPass = $row['Password'];
		$adminLog = $row['LastLogIn'];
		$adminIsRoot = $row['isRootAdmin'];
		$isAdmin = $row['isAdmin'];
		$adminStatus = $row['Active'];
		//verify and adjust entries
		$adminId = (int)$adminId;
		IF ($adminSSN == ""){$adminSSN = NULL;}
		ELSE{$adminSSN = (int)$adminSSN;}
		IF ($adminLog == ""){$adminLog = date_create()->format('Y-m-d H:i:s');}
		$adminIsRoot = (int)$adminIsRoot;
		$isAdmin = (int)$isAdmin;
		$adminStatus = (int)$adminStatus;
		$hashedPass = password_hash($adminPass, PASSWORD_DEFAULT);
		//echo($adminId . " " . $adminSSN. " ".$adminName. " ".$adminUser." " . $adminPass. " " . $adminLog. " ". $adminIsRoot. " " . $isAdmin . " " . $adminStatus ."<br>");
		$adminQuery = "INSERT IGNORE INTO 
		Admins (AID,ASSN,AName,Username,Password,LastLogIn,isRootAdmin,isAdmin,	Active)
		VALUES(?,?,?,?,?,?,?,?,?)";
		 $stmt = $conn->prepare($adminQuery);
		 $stmt->bind_param("iissssiii", $adminId, $adminSSN, $adminName, $adminUser,$hashedPass,$adminLog,$adminIsRoot,$isAdmin,$adminStatus);
		 $stmt->execute();
		 $stmt->close();

		
      }

}	

function decodeDoctorJSON($filePath){
	global $conn;
	$FileContents = file_get_contents($filePath);
	$array = json_decode($FileContents, true);
	
	foreach($array["Doctors"] as $row){
		$doctorId = $row['DID'];
		$doctorSSN = $row['DSSN'];
		$doctorName = $row['Dname'];
		$doctorUser = $row['Username'];
		$doctorPass = $row['Password'];
		$doctorLog = $row['LastLogIn'];
		$doctorStatus = $row['Active'];
		$canPerform = $row['SurgeryType'];
		//verify and adjust entries
		IF ($doctorSSN == ""){$doctorSSN = NULL;}
		IF ($doctorLog == ""){$doctorLog = date_create()->format('Y-m-d H:i:s');}
		$hashedPass = password_hash($doctorPass, PASSWORD_DEFAULT);
		 
		 $doctorQuery = "INSERT IGNORE INTO Doctors(DID,DSSN,Dname,Username,Password,LastLogIn,Active) 
		 VALUES (?,?,?,?,?,?,?)";
		 $stmt = $conn->prepare($doctorQuery);
		 $stmt->bind_param("iissssi", $doctorId, $doctorSSN, $doctorName, $doctorUser,$hashedPass,$doctorLog,$doctorStatus);
		 if(!$stmt->execute()){	 echo ("<script>console.log( 'Error inserting doctor' );</script>");}
		 $stmt->close();
		
		foreach ($canPerform as &$SurgeryType) {
			$SurgeryQuery  = "INSERT INTO Surgery(SDID,SurgeryType) VALUES(?,?)";
			$stmt = $conn->prepare($SurgeryQuery);
			$stmt->bind_param("is", $doctorId, $SurgeryType);
			if(!$stmt->execute()){ echo ("<script>console.log( 'Error inserting surgery type doctor can perform' );</script>");}
			$stmt->close();
		}
     }	
}	

function decodePatientsJSON($filePath){
	global $conn;
	$FileContents = file_get_contents($filePath);
	$array = json_decode($FileContents, true);
	
	foreach($array["Patients"] as $row){
		$PID = $row['PID'];
		$PSSN = $row['PSSN'];
		$PName = $row['PName'];
		$Username = $row['Username'];
		$Password = $row['Password'];
		$DOB = $row['DOB'];
		$LastLogIn = $row['LastLogIn'];
		$isPatient = $row['isPatient'];
		$Active = $row['Active'];
		$MedicalRecord =$row['Medical Record'];
		//verify and adjust entrie
		IF ($PSSN == ""){$PSSN = NULL;}
		IF ($LastLogIn == ""){$LastLogIn = date_create()->format('Y-m-d H:i:s');}
		$hashedPass = password_hash($Password, PASSWORD_DEFAULT);
		//patient
		$patientQuery = "INSERT IGNORE INTO Patients(PID,PSSN,PName,Username,Password,DOB,LastLogIn,isPatient,Active) VALUES (?,?,?,?,?,?,?,?,?)";
		$stmt = $conn->prepare($patientQuery);
		$stmt->bind_param("iisssssii", $PID, $PSSN, $PName, $Username,$hashedPass,$DOB,$LastLogIn,$isPatient,$Active);
		if(!$stmt->execute()){ echo ("<script>console.log( 'Error inserting patient' );</script>");}
		$stmt->close();
		//patient medical record
		$height = $MedicalRecord['HeightCM'];
		$weight = $MedicalRecord['WeightKG'];
		$Age = $MedicalRecord['Age'];
		$Blood = $MedicalRecord['BloodType'];
		$RecordQuery = "INSERT INTO MedicalRecord(MPID,heightCM,weightKG,age,BloodType) VALUES(?,?,?,?,?)";
		$stmt = $conn->prepare($RecordQuery);
		$stmt->bind_param("iiiis", $PID, $height, $weight,$Age,$Blood);
		if(!$stmt->execute()){ echo ("<script>console.log( 'Error inserting patient medical record' );</script>");}
		$stmt->close();
	}
}	

function decodeAppointmnetsJSON($filePath){
	global $conn;
	$FileContents = file_get_contents($filePath);
	$array = json_decode($FileContents, true);
	$indexer = NULL;
	foreach($array["Appointments"] as $row){
		$pid = $row['Apid'];
		$did = $row['Adid'];
		$appdate = $row['appDate'];
		$appStart = $row['AppStartTime'];
		$appEnd = $row['AppEndTime'];
		$appReason = $row['AppReason'];
		$appStatus = $row['AppStatus'];
		//appointment
		$appQuery = "INSERT IGNORE INTO Appointments(AppId,APID,ADID,AppDate,AppStartTime,AppEndTime,AppReason,AppStatus)VALUES(?,?,?,?,?,?,?,?)";
		$stmt = $conn->prepare($appQuery);
		$stmt->bind_param("iiisssss", $indexer, $pid, $did,$appdate,$appStart,$appEnd,$appReason,$appStatus);
		if(!$stmt->execute()){ echo ("<script>console.log( 'Error inserting patient medical record' );</script>");}		
		$stmt->close();
	
	}
}	
function decodeSurgeriesJSON($filePath){
	global $conn;
	$FileContents = file_get_contents($filePath);
	$array = json_decode($FileContents, true);
	$indexer = NULL;
	foreach($array["Surgeries"] as $row){
		$suregonId = $row['SDID'];
		$patientId = $row['SPID'];
		$sugeryDate = $row['SurgeryDate'];
		$sugeryStartTime = $row['SurgeryStartTime'];
		$sugeryEndTime = $row['SurgeryEndTime'];
		$sugeryType = $row['SurgeryType'];
		$sugeryRoom = $row['SurgeryRoom'];
		//SURGERY
		$sugeryQuery = "REPLACE INTO Surgeries(SID,SDID,SPID,SurgeryDate,SurgeryStartTime,SurgeryEndTime,SurgeryType,SurgeryRoom) VALUES (?,?,?,?,?,?,?,?)";
		$stmt = $conn->prepare($sugeryQuery);
		$stmt->bind_param("iiisssss", $indexer, $suregonId, $patientId,$sugeryDate,$sugeryStartTime,$sugeryEndTime,$sugeryType,$sugeryRoom);
		if(!$stmt->execute()){ echo ("<script>console.log( 'Error inserting surgery' );</script>");}			
		$stmt->close();
	}
}	
function decodeScheduleJSON($filePath){
	global $conn;
	$FileContents = file_get_contents($filePath);
	$array = json_decode($FileContents, true);
	
	foreach($array["Schedule"] as $row){
		$doctorId = $row['SDID'];
		$shiftDate = $row['ShiftDate'];
		$shiftBegin = $row['ShiftStartTime'];
		$shiftEnd = $row['ShiftEndTime'];	

		$shiftQuery = "REPLACE INTO Schedule(SDID,ShiftDate,ShiftStartTime,ShiftEndTime) VALUES (?,?,?,?)";
		$stmt = $conn->prepare($shiftQuery);
		$stmt->bind_param("isss", $doctorId, $shiftDate, $shiftBegin,$shiftEnd);
		if(!$stmt->execute()){ echo ("<script>console.log( 'Error inserting into doctors schedule' );</script>"	);}
		$stmt->close();
	}
}	
	
function decodeToolsJSON($filePath){
	global $conn;
	$FileContents = file_get_contents($filePath);
	$array = json_decode($FileContents, true);
	
	foreach($array["Tools"] as $row){
		$ToolName = $row['ToolName'];
		$ToolId = $row['ToolId'];
		$numTools = $row['Quantity'];
		$toolsQuery = "REPLACE INTO Tools(ToolName,ToolId,Quantity) VALUES (?,?,?)";
		$stmt = $conn->prepare($toolsQuery);
		$stmt->bind_param("sii", $ToolName, $ToolId, $numTools);
		if(!$stmt->execute()){ echo ("<script>console.log( 'Error inserting into Tools' );</script>");}
		$stmt->close();
	}
}	
function decodeToolsInUseJSON($filePath){
	global $conn;
	$FileContents = file_get_contents($filePath);
	$array = json_decode($FileContents, true);
	
	foreach($array["ToolsInUse"] as $row){
		$doctorUsingTool = $row['TDID'];
		$ToolId = $row['ToolId'];
		$isReturned = $row['ToolStatus'];
		$toolsInUseQuery = "REPLACE INTO ToolsInUse(TDID,ToolId,ToolStatus) VALUES (?,?,?)";
		$stmt = $conn->prepare($toolsInUseQuery);
		$stmt->bind_param("iis", $doctorUsingTool, $ToolId, $isReturned);
		if(!$stmt->execute()){ echo ("<script>console.log( 'Error inserting into ToolsInUse' );</script>");}		
		$stmt->close();
	}
}	
/*
decodeAdminJSON("SampleEntriesAdmin.json");
decodeDoctorJSON("SampleEntriesDoctors.json");
decodePatientsJSON("SampleEntriesPatients.json");
decodeAppointmnetsJSON("SampleEntriesAppointments.json");
decodeSurgeriesJSON("SampleEntriesSurgeries.json");
decodeScheduleJSON("SampleEntriesSchedule.json");
decodeToolsJSON("SampleEntriesTools.json");
decodeToolsInUseJSON("SampleEntriesToolsInUse.json");*/
header( 'Location: accountHome.html' ) ;
exit;
$conn->close();
?>