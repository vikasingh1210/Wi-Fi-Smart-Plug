 /*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include "ESP8266WiFi.h"
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include<Ticker.h>


MDNSResponder mdns;
WiFiServer server(80);
Ticker secondTick;
Ticker clockTick;

const char* ssid = "KD WF TS36";
String st;

//

const char* host = "kinedyna.in";
const char* streamId   = "....................";
const char* privateKey = "....................";
char info_array[40];
char youtube_subs[6];
////**time control variables***/////
volatile int clock_hour=0;
volatile int clock_minute=0;
volatile int clock_second=0;
int sum=0;
int setTime=0;
int control_time_loop=0;
int babalaboo=1;
int problem_due_to_unix_timestamp=0;
int hard_restart=0;
////**time control***/////
volatile int watchdogCount=0;
int read_info=0;
String esid;
String epass = "";
int buttonState = 0;
const int buttonPin = 16;   
const int redLED = 12; 
const int greenLED = 13;
const int blueLED = 14;
int limitssid=32;
int limitkey=96;

void setup() {
  delay(1000);
  pinMode(buttonPin, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  glow_none();
  delay(1000);
  glow_red();
  delay(1000);
  
  glow_green();
  delay(1000);
  
  glow_blue();
  delay(1000);
  
  glow_none();
  delay(1000);

  glow_yellow();
  delay(1000);

  glow_sky_blue();
  delay(1000);

  glow_magenta();
  delay(1000);

  glow_none();
  delay(1000);
  
  secondTick.attach(1,ISRwatchdog);
  clockTick.attach(1,ISRclock);
  Serial.begin(9600);
  EEPROM.begin(512);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.println("Startup");
  // read eeprom for ssid and pass
  Serial.println("Reading EEPROM ssid");
 
  for (int i = 0; i < limitssid; ++i)
    {
       
      char ch=char(EEPROM.read(i));
     if(ch=='%'){
                char special_ch=change_scepical_char(char(EEPROM.read(i+1)),char(EEPROM.read(i+2)));
                    i=i+2; 
                    esid+=special_ch;              
                
                Serial.println("yeda");     
            }
            else if(ch=='+')
            {
            char special_ch=' ';
             esid+=special_ch;
            }
            else
            esid +=ch;
    }
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");
  
  for (int i = 32; i < 96; ++i)
    {
      
      char ch=char(EEPROM.read(i));
      if(ch=='%'){
                char special_ch=change_scepical_char(char(EEPROM.read(i+1)),char(EEPROM.read(i+2)));
                    i=i+2; 
                    epass+=special_ch;   
                Serial.println("yeda");     
            }
            else if(ch=='+')
            {
            char special_ch=' ';
            esid+=special_ch;
            }
            else
            epass+=ch;
    }
  Serial.print("PASS: ");
  Serial.println(epass);  
  if ( esid.length() > 1 )
  WiFi.begin(esid.c_str(), epass.c_str());

  
 
  int connection_timout=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    connection_timout++;
    if(connection_timout>=20)
    {
    watchdogCount++;
    connection_timout=0;
    Serial.println(watchdogCount);
    }
    buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    watchdogCount=0;
    set_ssid_password();
    
    
  } 
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  
}

int value = 0;

void loop() {
  //Serial.println("Wacthdog timer count is");
  //Serial.println(watchdogCount);
  watchdogCount=0;
  delay(1000);
  ++value;
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    set_ssid_password();
    
  } 
   /*Serial.print("hours:");
    Serial.println(clock_hour);
    Serial.print("Minutes:");
    Serial.println(clock_minute);
    Serial.print("second:");
    Serial.println(clock_second);
    Serial.print("control_time_loop");
    Serial.println(control_time_loop);*/
  if((clock_hour <= 0) && (clock_minute <= 0)&&(control_time_loop==1)&&(problem_due_to_unix_timestamp==0))//39020202 sbi
     {
        /*digitalWrite(13, LOW);                
                                glow_magenta();*/
                                control_time_loop=0;    
                                Serial.println("TIMER MODE SAAAAAAAAAAAAAAALLLLLLLLLAAAA");
                                glow_red();
                                
                                  
        }
  
 // Serial.print("connecting to ");
 // Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    problem_due_to_unix_timestamp=0;
    hard_restart++;
    if(hard_restart>=10)
    {
      hard_restart=0;
    ESP.reset();    
    }
    
    return;
  }
  
  // We now create a URI for the request
  /*String url = "/input/";
  url += streamId;
  url += "?private_key=";
  url += privateKey;
  url += "&value=";
  url += value;*/
  String url="http://kinedyna.in/75DJHFGPASMK/receive.php";
  
 // Serial.print("Requesting URL: ");
  //Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  int timeout = millis() + 5000;
  while (client.available() == 0) {
    if (timeout - millis() < 0) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    //Serial.print(line);
        read_info=line.indexOf("kinedyna");
        //Serial.println(read_info);
            if(read_info>=1)
            {
              hard_restart=0;
              //serial.println("collecting info");
              read_info=0;
             line.toCharArray(info_array,30);

   /*Serial.println(info_array[9]);
   Serial.println(info_array[10]);
   Serial.println(info_array[11]);*/
   int i;
   char c;
   for(i=0;i<5;i++)
    {
      c=info_array[10+i];
      if(c!='J')
      {
        //Serial.println(c);
      youtube_subs[i]=info_array[10+i];
     problem_due_to_unix_timestamp=1;
  //Serial.print(youtube_subs[i]);
  /*Serial.print(i);
  Serial.print("]");*/
  //Serial.println(youtube_subs[i]);
        }
        else
        break;
      
      }

       

  if((youtube_subs[1]=='N'))
        {
          Serial.println("ON");
          control_time_loop=0;
          //clockTick.detach();
          glow_green();

        }
        else if((youtube_subs[2]=='F')&&(youtube_subs[0]=='O'))
        {
         
         Serial.println("OFF");
          control_time_loop=0;
          //clockTick.detach();
          glow_red();
        }
        else
        {
          control_time_loop=1;
          //clockTick.attach(1,ISRclock);
          sum=0;    
          for(babalaboo=0; babalaboo<i; babalaboo++)
        {
          char y=youtube_subs[babalaboo];
          int k=y-'0';
          sum=sum*10+k;
          //Serial.println("Value of i is");
          //Serial.println(i);
          //Serial.println("Value of sum is");
          
          //Serial.println(y);
        }
          Serial.print("TIMER");
          Serial.print(sum);
          Serial.println("JANE");
          setTime=sum;
          clock_hour=setTime/60;
          clock_minute=setTime%60;
          //serial.println(clock_hour);
          //serial.println(clock_minute);
         glow_blue();


        }
              
            }
       
   
  }
  
  //serial.println();
  //serial.println("closing connection");
   //serial.println("Dude lets go this last");
   
   
   
}



int testWifi(void) {
  int c = 0;
  Serial.println("Waiting for Wifi to connect");  
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED) { return(20); } 
    delay(500);
    Serial.print(WiFi.status());    
    c++;
  }
  Serial.println("Connect timed out, opening AP");
  return(10);
} 

void launchWeb(int webtype) {
          //serial.println("");
          Serial.println("WiFi connected");
          Serial.println(WiFi.localIP());
          Serial.println(WiFi.softAPIP());
          ///////////////********************////////////
         /*if (!mdns.begin("esp8266", WiFi.localIP())) {
            Serial.println("Error setting up MDNS responder!");
            while(1) { 
              delay(1000);
            }
          }*/
          ///////////////////***********************//////////////
          Serial.println("mDNS responder started");
          // Start the server
          server.begin();
          Serial.println("Server started");   
         glow_magenta();
          int b = 20;
          int c = 0;
          ////************if button is high run this********************//////////
          while(b == 20) { 
             b = mdns1(webtype);
             watchdogCount=0;
           }
           
          ////************if button is high run this********************//////////
}

void setupAP(void) {
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
     {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
     }
  }
  //serial.println(""); 
  st = "<ul>";
  for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      st += "<li>";
      st +=i + 1;
      st += ": ";
      st += WiFi.SSID(i);
      st += " (";
      st += WiFi.RSSI(i);
      st += ")";
      st += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*";
      st += "</li>";
    }
  st += "</ul>";
  delay(100);
  WiFi.softAP(ssid);
  Serial.println("softap");
  Serial.println("");
  launchWeb(1);
  Serial.println("over");
}

int mdns1(int webtype)
{
  // Check for any mDNS queries and send responses
  mdns.update();
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return(20);
  }
  //serial.println("");
  Serial.println("New client");

  // Wait for data from client to become available
  while(client.connected() && !client.available()){
    delay(1);
   }
  
  // Read the first line of HTTP request
  String req = client.readStringUntil('\r');
  
  // First line of HTTP request looks like "GET /path HTTP/1.1"
  // Retrieve the "/path" part by finding the spaces
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  if (addr_start == -1 || addr_end == -1) {
    Serial.print("Invalid request: ");
    Serial.println(req);
    return(20);
   }
  req = req.substring(addr_start + 1, addr_end);
  Serial.print("Request: ");
  Serial.println(req);
  client.flush(); 
  String s;
  if ( webtype == 1 ) {
      if (req == "/")
      {
        IPAddress ip = WiFi.softAPIP();
        String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
        s += ipStr;
        s += "<p>";
        s += st;
        s += "<form method='get' action='a'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
        s += "</html>\r\n\r\n";
        Serial.println("Sending 200");
      }
      else if ( req.startsWith("/a?ssid=") ) {
        // /a?ssid=blahhhh&pass=poooo
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }
        String qsid; 
        qsid = req.substring(8,req.indexOf("AZPG821"));
        Serial.println(qsid);
        Serial.println("");
        String qpass;
        qpass = req.substring(req.lastIndexOf('=')+1);
        Serial.println(qpass);
        Serial.println("");
        
        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
          {
            EEPROM.write(i, qsid[i]);
            Serial.print("Wrote: ");
            Serial.println(qsid[i]); 
          }
        Serial.println("writing eeprom pass:"); 
        for (int i = 0; i < qpass.length(); ++i)
          {
            EEPROM.write(32+i, qpass[i]);
            Serial.print("Wrote: ");
            Serial.println(qpass[i]); 
          }    
        EEPROM.commit();
        s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><title>75DJHFGPASMK</title>";
        s += "Hello from ESP8266 Found ";
        s += req;
        s += "<p> saved to eeprom... reset to boot into new wifi</html>\r\n\r\n";
        
      }
      else
      {
        s = "HTTP/1.1 404 Not Found\r\n\r\n";
        Serial.println("Sending 404");
      }
  } 
  else
  {
      if (req == "/")
      {
        s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP8266";
        s += "<p>";
        s += "</html>\r\n\r\n";
        Serial.println("Sending 200");
      }
      else if ( req.startsWith("/cleareeprom") ) {
        s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP8266";
        s += "<p>Clearing the EEPROM<p>";
        s += "</html>\r\n\r\n";
        Serial.println("Sending 200");  
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }
        EEPROM.commit();
      }
      else
      {
        s = "HTTP/1.1 404 Not Found\r\n\r\n";
        Serial.println("Sending 404");
      }       
  }
  client.print(s);
  Serial.println("Done with client");
  ESP.reset();
  return(20);
}

void ISRwatchdog(){
  watchdogCount++;
  if(watchdogCount>=50){
    Serial.println();
    Serial.println("Kala Kuta kat khaya");
    ESP.reset();
    }
  
  }

void set_ssid_password()
{
  
  if ( esid.length() > 1 ) {
     
      
      if ( testWifi() == 20 ) { 
          launchWeb(1);
          return;
      }
  }
  
   setupAP(); //call this when button is high than start this
  
  }

 void ISRclock(){
  clock_second++;
    
    if(clock_second==60)
    {
      clock_minute--;
      clock_second=0;

      if(clock_minute==-1)
      {
        clock_hour--;
        clock_minute=59;
      }
    }
  
  }
  void glow_blue()
  {
    digitalWrite(redLED, LOW);   
  digitalWrite(greenLED, HIGH);
  digitalWrite(blueLED, HIGH);   
    }
  
  void glow_green()//
  {
    
  
  digitalWrite(blueLED, LOW);   
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, HIGH);   
  
  }
  
  void glow_red()
  {
    
  digitalWrite(greenLED, LOW);   
  digitalWrite(blueLED, HIGH);
  digitalWrite(redLED, HIGH);   
  
  }
  
  
  void glow_white()
  {
    
  digitalWrite(greenLED, LOW);   
  digitalWrite(blueLED, LOW);
  digitalWrite(redLED, LOW);   
  
  }
  
  void glow_none()
  {
    
  digitalWrite(greenLED, HIGH);   
  digitalWrite(blueLED, HIGH);
  digitalWrite(redLED, HIGH);   
  
  }
  
  void glow_yellow()
  {
    digitalWrite(redLED, HIGH);   
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, LOW);   
    }
    
    void glow_magenta()
  {
    digitalWrite(redLED, LOW);   
  digitalWrite(greenLED, HIGH);
  digitalWrite(blueLED, LOW);   
    }
    
    void glow_sky_blue()
  {
    digitalWrite(redLED, LOW);   
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, HIGH);   
    }


    char change_scepical_char(char a, char b)
  {   
    
    if((a=='4')&&(b=='0'))
    return ('@');
    
    if((a=='2')&&(b=='1'))
    return ('!');
    
    if((a=='2')&&(b=='2'))
    return ('"');
    
    if((a=='2')&&(b=='3'))
    return ('#');
    
    if((a=='2')&&(b=='4'))
    return ('$');

    
    if((a=='2')&&(b=='5'))
    return ('%');  
  
  
    if((a=='2')&&(b=='6'))
    return ('&');
  
    if((a=='2')&&(b=='7'))
    return ('\'');
  
    if((a=='2')&&(b=='8'))
    return ('(');
  
    if((a=='2')&&(b=='9'))
    return (')');
  
    if((a=='2')&&(b=='A'))
    return ('*');
    if((a=='2')&&(b=='B'))
    return ('+');
  
    if((a=='2')&&(b=='C'))
    return (',');
  
    if((a=='2')&&(b=='D'))
    return ('-');
  
    if((a=='2')&&(b=='E'))
    return ('.');
  
    if((a=='2')&&(b=='F'))
    return ('/');
    
    if((a=='3')&&(b=='A'))
    return (':');
    
    if((a=='3')&&(b=='B'))
    return (';');
    
    if((a=='3')&&(b=='C'))
    return ('<');
    
    if((a=='3')&&(b=='D'))
    return ('=');
    
    if((a=='3')&&(b=='E'))
    return ('>');
    
    if((a=='3')&&(b=='F'))
    return ('?');
    
    if((a=='5')&&(b=='B'))
    return ('[');
    
    if((a=='5')&&(b=='C'))
    return ('\'');
    
    if((a=='5')&&(b=='D'))
    return (']');
    
    if((a=='5')&&(b=='E'))
    return ('^');
    
    if((a=='5')&&(b=='F'))
    return ('_');
    
    if((a=='6')&&(b=='0'))
    return ('`');
  
    if((a=='7')&&(b=='B'))
    return ('{');
    
     if((a=='7')&&(b=='C'))
    return ('|');
    
     if((a=='7')&&(b=='D'))
     return ('}');
     
     if((a=='7')&&(b=='E'))
     return ('~');
     
     else
     return('0');

  
  }

