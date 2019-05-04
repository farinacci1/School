<?php
session_start();
if($_SERVER["REQUEST_METHOD"] == "POST") {
    if (isset($_POST['aDoc'])) {
       $_SESSION['selectednavBarOption'] = "aDoc";
    } 	
    elseif (isset($_POST['aPat'])) {
       $_SESSION['selectednavBarOption'] = "aPat";
    }
    elseif (isset($_POST['aReq'])) {
       $_SESSION['selectednavBarOption'] = "aReq";
    } 		
    elseif (isset($_POST['aAoc'])) {
       $_SESSION['selectednavBarOption'] = "aAoc";
    } 		
    elseif (isset($_POST['dDoc'])) {
       $_SESSION['selectednavBarOption'] = "dDoc";
    }
    elseif (isset($_POST['dPat'])) {
       $_SESSION['selectednavBarOption'] = "dPat";
    } 			
    elseif (isset($_POST['dAoc'])) {
       $_SESSION['selectednavBarOption'] = "dAoc";
    } 			
    elseif (isset($_POST['pApp'])) {
       $_SESSION['selectednavBarOption'] = "pApp";
    }
    elseif (isset($_POST['pMR'])) {
       $_SESSION['selectednavBarOption'] = "pMR";
    } 		
    elseif (isset($_POST['pSHD'])) {
       $_SESSION['selectednavBarOption'] = "pSHD";
    } 		
    elseif (isset($_POST['pAoc'])) {
       $_SESSION['selectednavBarOption'] = "pAoc";
    } 
	
	header("Location: accountHome.html");
	exit;
}

?>