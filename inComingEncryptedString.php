Back (accesskey b)    Save (accesskey s)    	File: /public_html/timeshort/web2file.php
 	Status: This file has not yet been saved
 

<?php


function get_string_between($string, $start, $end){
    $string = ' ' . $string;
    $ini = strpos($string, $start);
    if ($ini == 0) return '';
    $ini += strlen($start);
    $len = strpos($string, $end, $ini) - $ini;
    return substr($string, $ini, $len);
}


/*
 * Written By: Taifun (by using the Web2SQL example from ShivalWolf)
 * Date: 2011/12/18
 * Contact: info@puravidaapps.com
 */

 /************************************CONFIG****************************************/

//SETTINGS//
//This code is something you set in the APP so random people cant use it.
$ACCESSKEY="secret";

/************************************CONFIG****************************************/

//these are just in case setting headers forcing it to always expire and the content type to CSV

//header('Cache-Control: no-cache, must-revalidate');
//header('Content-type: text/csv');



if( isset($_POST['export']) && isset($_POST['key']) ){  // checks if the tag post is there and if its been a proper form post
  if($_POST['key']==$ACCESSKEY){                        // validate the ACCESS key
    $export=urldecode($_POST["export"]);		//incoming encrypted data from app is  $export
    
    
    //decrypt the data and check the time stamp
   
    
//connecting to server with credentials
$servername = "localhost";
$username = "viki321";
$password = "leenab321";
$dbname = "vikiESP";

$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) 
{
    die("Connection failed: " . $conn->connect_error);
} 
//getting LINK for mySQL query 


//$LINKaddress=get_string_between($_SERVER['REQUEST_URI'], '/', '/');//don't use'/' in name of smart plug
//have to write manualy beacuse ESP is not able to retrive it browser can so be awere
$LINKaddress="aaba0014"; 

$sql = "SELECT SENSOR FROM ESPtable WHERE LED='$LINKaddress'";
$result = $conn->query($sql);


if ($result->num_rows > 0) 
{
	
	while($row = $result->fetch_assoc()) 
	{
	       // echo "SENSOR: " . $row["SENSOR"] ."<br>";
	       $encodingString=$row["SENSOR"];
	}
}
else
{

 $encodingString = "vikassingh!@#123"; //default encryptionkey
}

echo $encodingString;/////////////////////////////
echo "<br>";//////////////////////

    $hex='';
    for ($i=0; $i < strlen($encodingString); $i++){
        $hex .= dechex(ord($encodingString[$i]));
    }
	  
	
	$key2 = pack("H*", $hex);
	$iv =  pack("H*", $hex);
    
	    
	$encrypted = base64_decode(urldecode($export));//decrypting the text
	
	echo $encrypted;//////////////////////////
	$shown = mcrypt_decrypt(MCRYPT_RIJNDAEL_128, $key2, $encrypted, MCRYPT_MODE_CBC, $iv);//decrypting the text
	echo "<br>";////////////////////
	echo $shown;//////////////////////////////////////
	$shown = trim($shown,"\0");
	$shown = trim($shown,"\t");
	$shown = trim($shown,"\n");
	$shown = trim($shown,"\x0B");
	$shown = trim($shown,"\r");
	$shown = trim($shown," ");
	$shown = stripslashes($shown);
	$shown = htmlspecialchars($shown);
	echo $shown."<br>";
	
	$fullstring = $shown;
	$parsed = get_string_between($fullstring, 'KW01start', 'KW01end');//taking out the Time Stamp from incoming data
		
	$myfileReadTS = fopen("TimeStampOfCommand.txt", "r") or die("Unable to open file!");//reading the previous time stamp 
	$ReadingTS=(string)file_get_contents('TimeStampOfCommand.txt');
	//echo $dataTF;
	fclose($myfileReadTS);
	
	if($parsed>$ReadingTS)//comapring the new time stamp with previous time stamp if true than it will re write the data else it will stop the replay attack
	{

	   
	   
	    $outstream = fopen("export.txt", "w");// writing complete encrypted text to export.text	   
	    fwrite($outstream,  $export);      //writing data to export file and "lastoption.php" will analize the data and forcat the command 
	    fclose($outstream);
	    
	    //write contents in repective files
	    
	    $parsed = get_string_between($fullstring, 'KW01start', 'KW01end');//taking out the Time Stamp from incoming data
	    $outstream = fopen("TimeStampOfCommand.txt", "w");// writing new Time Stamp text to TimeStampOfCommand.text	   
	    fwrite($outstream,$parsed);      //writing data to TimeStampOfCommand file and "lastoption.php" will analize the data and forcat the command 
	    fclose($outstream);
	    
	    //writing command to command.txt
	    $parsed = get_string_between($fullstring, 'KW02start', 'KW02end');//taking out the Time Stamp from incoming data
	    $outstream = fopen("command.txt", "w");// writing complete encrypted text to export.text	   
	    fwrite($outstream,$parsed);      //writing data to export file and "lastoption.php" will analize the data and forcat the command 
	    fclose($outstream);
	   
	   
	    //writing counter to counter.txt
	    $parsed = get_string_between($fullstring, 'KW03start', 'KW03end');//taking out the Time Stamp from incoming data
	    $outstream = fopen("counter.txt", "w");// writing complete encrypted text to export.text	   
	    fwrite($outstream,$parsed);      //writing data to export file and "lastoption.php" will analize the data and forcat the command 
	    fclose($outstream);
	    echo $shown;                                       // writes out csv data back to the client
	    
	    $parsed = get_string_between($fullstring, 'KW04start', 'KW04end');//taking out the Time Stamp from incoming data
	    $outstream = fopen("startingPoint.txt", "w");// writing complete encrypted text to export.text	   
	    fwrite($outstream,$parsed);      //writing data to export file and "lastoption.php" will analize the data and forcat the command 
	    fclose($outstream);
	    echo $shown;   
	    
	    $url1=$_SERVER['REQUEST_URI'];
	    header("Refresh: 10; URL=$url1");

	}



} 
else
 {
    header("HTTP/1.0 400 Bad Request");
    echo "Bad Request";                                 // reports if the code is bad
  }
} 
else 
{
  header("HTTP/1.0 400 Bad Request");
  echo "Bad Request123";
  

}
?>