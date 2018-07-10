<?php

//Here we have the key and iv which we know, because we have just chosen them on the JS,
//the pack acts just like the parse Hex from JS

    $string = "vikassingh!@#123";
    $hex='';
    for ($i=0; $i < strlen($string); $i++){
        $hex .= dechex(ord($string[$i]));
    }
  

$key = pack("H*", $hex);
$iv =  pack("H*", $hex);


//Now we receive the encrypted from the post, we should decode it from base64,

$encrypted = base64_decode($_GET["encrypted"]);
$shown = mcrypt_decrypt(MCRYPT_RIJNDAEL_128, $key, $encrypted, MCRYPT_MODE_CBC, $iv);
	$shown = trim($shown,"\0");
	$shown = trim($shown,"\t");
	$shown = trim($shown,"\n");
	$shown = trim($shown,"\x0B");
	$shown = trim($shown,"\r");
	$shown = trim($shown," ");
	$shown = stripslashes($shown);
	$shown = htmlspecialchars($shown);
	
echo $shown;
echo "fromphp"


?>