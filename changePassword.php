<?php


//header('Cache-Control: no-cache, must-revalidate');
//header('Content-type: text/csv');

$servername = "localhost";
$username = "viki321";
$password = "leenab321";
$dbname = "vikiESP";

//$var1 = $_GET['passwordKey'];//user defined encryption key
//$id1 = $_GET['LINKaddress'];//linkaddress is unique name of smart plug 

/*********************post method which going to be final******************/////////////

$ACCESSKEY="secret";

if(isset($_POST['key']) && isset($_POST['passwordKey'])&&isset($_POST['LINKaddress']))
{
	if($_POST['key']==$ACCESSKEY)
	{
		$var1=urldecode($_POST["passwordKey"]);
		$id1=urldecode($_POST["LINKaddress"]);		
	}	
	/*********************post method which going to be final******************/////////////
	echo $var1;
	echo $id1;
	echo "Hey dude"."<br>";
	// Create connection
	$conn = new mysqli($servername, $username, $password, $dbname);
	// Check connection
	if ($conn->connect_error) 
	{
		die("Connection failed: " . $conn->connect_error);
	}
	
	$sql = "UPDATE ESPtable SET SENSOR='{$var1}' WHERE LED ='$id1' ";
	
	if ($conn->query($sql) === TRUE) 
	{
		echo "Record updated successfully kindin";
	} 
	else 
	{
		echo "Error updating record: " . $conn->error;
	}
	
	$conn->close();
}

else 
{
	header("HTTP/1.0 400 Bad Request");
	echo "Bad Request123";
}

?>