<?php

//FDG
//DFG
//RET

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
  
    
//function to find the info/data between two keywords  
function get_string_between($string, $start, $end)
{
    $string = ' ' . $string;
    $ini = strpos($string, $start);
    if ($ini == 0) return '';
    $ini += strlen($start);
    $len = strpos($string, $end, $ini) - $ini;
    return substr($string, $ini, $len);
}

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
	
	$myfile = fopen("export.txt", "r") or die("Unable to open file!");
	$dataFromExport=file_get_contents('export.txt');//storing encrypted data from file 
	
	 //$encodingString = "vikassingh!@#123"; //encryptionkey is obtained above
	$hex='';
	for ($i=0; $i < strlen($encodingString); $i++)
	{
		$hex .= dechex(ord($encodingString[$i]));
	}
		  
	
	$key2 = pack("H*", $hex);
	$iv =  pack("H*", $hex);
	$encrypted = base64_decode(urldecode($dataFromExport));//decrypting the text
	$shown = mcrypt_decrypt(MCRYPT_RIJNDAEL_128, $key2, $encrypted, MCRYPT_MODE_CBC, $iv);//decrypting the text
	$shown = trim($shown,"\0");
	$shown = trim($shown,"\t");
	$shown = trim($shown,"\n");
	$shown = trim($shown,"\x0B");
	$shown = trim($shown,"\r");
	$shown = trim($shown," ");
	$shown = stripslashes($shown);
	$shown = htmlspecialchars($shown);
	$fullstring = $shown;
	$parsed = get_string_between($fullstring, 'KW02start', 'KW02end');//taking out the command from incoming data NOW no need of writing to command.txt
	$dude=$parsed ;
	fclose($myfile);	
	if($dude=="ON")
	{
		echo"kinedynaON Jane";
		$dude1= intval($dude);
	
	}
	elseif($dude=="OFF")
	{
		echo"kinedynaOFF Jane";
		$dude1= intval($dude);
	
	}
	elseif(strpos($dude,"ZJP")>=1)
	{
		echo("Schedular Mode");
	}

	else
	{		
		$dude1= intval($dude); 
		$dude1=floor($dude1/1000);  
		$current_time= time();
		if($dude1<=$current_time)
		{
			echo"Switched OFF";
		}
		else
		{
			echo"Switch Off in ";
			echo $diff=floor(($dude1-$current_time)/60)+1;
			echo" Minutes";
		}
        
        
	}
}

?>

			