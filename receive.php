<?php

//getting the sensor data from sPlug
$var1 = $_GET['apples'];
$var2 = $_GET['oranges'];
$var3 = $var1;
$var4 = $var2;


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
	    
	 
	
	$previous=fopen("myFile.txt", "r")or die("Unable to open file!");//opening myfile to read old data
	$get_p_reading1=file_get_contents('myFile.txt');  //storing old data
	
	
	fclose($previous);
	
	$outs = fopen("myFile.txt", "w");//againg open myfile to write newly data
	$time_entry = fopen("time_entry_SP.txt","w");
	$write_unix_TS=time();
	fwrite($time_entry,$write_unix_TS);
	fclose($time_entry);
	$fileContent=$var3*$var4+($get_p_reading1);// writing new data
	//echo("\n dude this is sum reading".$fileContent);
	
	fwrite($outs, $fileContent);
	  
	fclose($outs);
	
	
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
	
	$repeater = strpos($dude,"ZJP");// if its in shedular mode
	
	$data_collector_ON="";
	$data_collector_OFF="";
	$data_collector_Repeater="";
	fclose($myfile);
	if($outs!=false)
	{
		if($dude=="ON")
		{
			echo" kinedynaONJane";
			$dude1= intval($dude);
		
		}
		elseif($dude=="OFF")
		{
			echo" kinedynaOFFJane";
			$dude1= intval($dude);
		
		}
	        elseif($repeater>=1)
	        {
		$repeaterFile = fopen("startingPoint.txt", "r") or die("Unable to open file!");
		$StartingPointTime=file_get_contents('startingPoint.txt');// this is starting point file
		
		
		$t=time();
		
		$day=date("l");
		
		fclose($repeaterFile);
		
	         $arr1 = str_split($dude);
	        
	        
	         $selector=0;
	         $i=1;
	         $z=1; 
	         $zR=1;
	         $multiplyerToTakeCareZero=1;
	         for($j=0;$j<40;$j++)
	           {
	                    if($arr1[$j]=="P")//after 'P' we have data for ON value
	                    $selector=1;
	                    
	                    if($arr1[$j]=="A")//after 'A' we have data for OFF value
	                    $selector=2;
	                    
	                    if($arr1[$j]=="R")//after 'R' we have data for no of repeatition  value
	                    $selector=3;
	                    
	                    if($arr1[$j]=="E")// once we found 'E' we have to break from loop
	                    break;
	                                     
	                    if($selector==1)    //data will be store for ON value
	                    {                    
		                    $data_collector_ON=$data_collector_ON+($arr1[$j+1])*$i;
		                    $i=$i*10;
		                    //echo("Value of dataON");
		                    //echo intval($data_collector_ON);	                    
	                    }
	                     if($selector==2)     //data will be store for OFF value
	                    {                                   
		                    $data_collector_OFF=$data_collector_OFF+($arr1[$j+1])*$z;
		                    $z=$z*10;
		                    //echo("Value of dataOFF");
		                    //echo intval($data_collector_OFF);                    
	                    }
	                    if($selector==3)    //data will be store for no of repeatition value
	                    {         
	                    	    if($arr1[$j+1]=="-")
	                    	    {
	                    	    	echo "kinedynaOFFJane";
	                    	    	$multiplyerToTakeCareZero=0;
	                    	    }
	                    	    else
	                    	    { 
			                    //echo ($arr1[$j+1]);
			                    $data_collector_Repeater=$data_collector_Repeater+($arr1[$j+1])*$zR;
			                    $zR=$zR*10;
		                    }
		                    //echo("Value of Repeater");
		                                 
	                    }
	                    
	                    
	           }
	           //$data_collector_Repeater =$data_collector_Repeater*$multiplyerToTakeCareZero;
	           
	           //echo("this the data");
	           $act_data_ON=strrev($data_collector_ON);//time period of ON mins. We are reversing it thats why there is a problem of ZERO
	           $act_data_OFF=strrev($data_collector_OFF);//time period of OFF mins.
	           $act_data_Repeater=strrev($data_collector_Repeater);//time period of OFF mins.
	           $act_data_Repeater=$act_data_Repeater*$multiplyerToTakeCareZero;
	           //echo $act_data_Repeater;
	           $current_time= time();
	           //echo("current_time");
	           //echo($current_time);
	           //echo("Starting Point Time");
	           //echo($StartingPointTime);
	           $d=floor(($current_time-$StartingPointTime)/60);//diff in mins
	                  
	           $diff_ON=($act_data_ON-floor(($current_time-$StartingPointTime)/60));//d1-d
	           $diff_OFF=($act_data_OFF-floor(($current_time-$StartingPointTime)/60));//d2-d
	           //echo("Diff in ON TIME");
	           //echo($diff_ON);
	           if($diff_ON>0)
	           {
	           //echo($StartingPointTime);
	           //echo("Machha diya bhai");
	           }
	           /*for($i=1;$i<$act_data_Repeater;$i++)
	           	{
	           	if((($i*$act_data_ON-$d)>0)&&((($i-1)*$act_data_OFF-$d)<0))
	           		{
	           		echo("BiDDDUUUUUUU in ON TIME ZONE");
	           		echo("Biddu On TIME--->");
	           		echo($i*$act_data_ON-$d);
	           		echo("Biddu Off TIME--->");
	           		echo(($i-1)*$act_data_OFF-$d);
	           		echo("VALUE OF i IS".$i);
	           		break;
	           		}
	           		
	           	elseif((($i*$act_data_ON-$d)<=0)&&((($i)*$act_data_OFF-$d)>=0))
	           		{
	           		echo("Biddu Off ZONE mein hain");
	           		break;
	           		}
	           	elseif(((($act_data_ON+$act_data_OFF)*$act_data_Repeater)-$d)<0)
	           		{
	           		echo("Bazaar Khatm");
	           		echo(((($diff_ON+$diff_OFF)*$act_data_Repeater)-$d));
	           		echo("Bazaar Khatm");
	           		break;
	           		}
	           		
	           	
	           	}*/
	           	//echo $act_data_Repeater;
	           	for($i=1;$i<=$act_data_Repeater;$i++)
	           	{
	           		//echo("Value of i-->>>".$i);
	           		if(($i*($act_data_ON+$act_data_OFF)-$act_data_OFF-$d)>0)//$d is diffrence in mins between current time - starting point
	           		{
	           			if($d<0)
	           			{
		           			echo(" kinedynaOFFJane");
		           			echo($d);
		           			echo("Jone future");
		           			break;
	           			}
	           			if($repeater = strpos($dude,"aysch")>0)//this IF is for daywise schedular 
	           			{
	           				    			
		           			if($day=="Monday")
		           			{
			           			if($dayfinder = strpos($dude,"mo")>0)
			           			echo(" kinedyna");
			           			echo(($i*($act_data_ON+$act_data_OFF)-$act_data_OFF-$d));
			           			echo("JaneMo");
			           			break;
		           			}
		           			
		           			if($day=="Tuesday")
		           			{
			           			if($dayfinder = strpos($dude,"tu")>0)
			           			echo(" kinedyna");
			           			echo(($i*($act_data_ON+$act_data_OFF)-$act_data_OFF-$d));
			           			echo("JaneTu");
			           			break;
		           			}
		           			
		           			if($day=="Wednesday")
		           			{
			           			if($dayfinder = strpos($dude,"we")>0)
			           			echo(" kinedyna");
			           			echo(($i*($act_data_ON+$act_data_OFF)-$act_data_OFF-$d));
			           			echo("JaneWed");
			           			break;
		           			}
		           			
		           			if($day=="Thursday")
		           			{
			           			if($dayfinder = strpos($dude,"th")>0)
			           			echo(" kinedyna");
			           			echo(($i*($act_data_ON+$act_data_OFF)-$act_data_OFF-$d));
			           			echo("JaneTh");
			           			break;
		           			}
		           			
		           			if($day=="Friday")
		           			{
			           			if($dayfinder = strpos($dude,"fr")>0)
			           			echo(" kinedyna");
			           			echo(($i*($act_data_ON+$act_data_OFF)-$act_data_OFF-$d));
			           			echo("JaneFr");
			           			break;
		           			}
		           			
		           			if($day=="Saturday")
		           			{
			           			if($dayfinder = strpos($dude,"sa")>0)
			           			echo(" kinedyna");
			           			echo(($i*($act_data_ON+$act_data_OFF)-$act_data_OFF-$d));
			           			echo("JaneSa");
			           			break;
		           			}
		           			
		           			if($day=="Sunday")
		           			{
			           			if($dayfinder = strpos($dude,"su")>0)
			           			echo(" kinedyna");
			           			echo(($i*($act_data_ON+$act_data_OFF)-$act_data_OFF-$d));
			           			echo("JaneSu");
			           			break;
		           			}
	           			
	           			}
	           			else
	           			{
		           			echo(" kinedyna");
		           			echo(($i*($act_data_ON+$act_data_OFF)-$act_data_OFF-$d));
		           			echo("Jane");           			
		           			//echo("Value that is needed is----->");
		           			//echo(($i*($act_data_ON+$act_data_OFF)-$act_data_OFF-$d));
		           			//echo("Value of i is----->");
		           			echo("d is ++ve Repeater");
		           			break;
	           			}
	           		}
	           		elseif(($i*($act_data_ON+$act_data_OFF)-$d)>0)
	   			{
	           			echo(" kinedynaOFFJane");
	           			
	           			echo("d is ++ve Repeater");
	           			break;
	   			}
	           			
	           		elseif(($act_data_Repeater*($act_data_ON+$act_data_OFF)-$d)<0)
	   			{
	           			echo("IN BAAAAAAZZZZZAAAAAAR KHATM MODE");
	           			break;
	   			}          		
	           		
	           	}   
	           
	         }//end of all shedular mode
		else
		{	
			if (is_numeric($dude)) // remove this check either text with encryption is right
			{
				$dude1= intval($dude); 
			        $dude1=floor($dude1/1000);       
			        $current_time= time();       
			        if($dude1<=$current_time)
			        {
			              echo" kinedynaOFFJane";
			        }
			        else
			        {
			             echo" kinedyna";
			             echo $diff=floor(($dude1-$current_time)/60)+1;
			             echo"Jane";
			        }
		        
		        }
		}
		
	}
	else
	{
		echo"FAIL";
	}
	$conn->close();
}


else {
  header("HTTP/1.0 400 Bad Request");
  echo "Bad Request123vikias";
  }
 
 ?>
 
 
 