/*
  Serial Event example

 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and
 clears it.

 A good test for this is to try it with a GPS receiver
 that sends out NMEA 0183 sentences.

 Created 9 May 2011
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/SerialEvent

 */
#include <avr/io.h>
#include <avr/interrupt.h>

int hard_restart=0;
int led1=5;
int led2=6;
int led3=8;
int connection_failed_problem=0;
int relay_control_pin=7;
int esp_power_supply_pin=9;
int setTime=0;
int clock_hour=0;
int clock_minute=0;
int clock_second=0;
int control_time_loop=0;
int problem_due_to_unix_timestamp=0;
String sum="";
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
char istring[20];
void setup() {
  // initialize serial:
  pinMode(relay_control_pin, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(esp_power_supply_pin, OUTPUT);
  digitalWrite(esp_power_supply_pin,HIGH);
  
   digitalWrite(led1,HIGH);
   digitalWrite(led2,LOW);
   digitalWrite(led3,HIGH);
   delay(1000);
   digitalWrite(led2,HIGH);
   digitalWrite(led3,LOW);
   digitalWrite(led1,HIGH);
   delay(1000);
   digitalWrite(led3,HIGH);
   digitalWrite(led1,LOW);
   digitalWrite(led2,HIGH);
   delay(1000);
  
  
  Serial.begin(19200);//19200
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  digitalWrite(esp_power_supply_pin,LOW);
  cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
    
    // set compare match register to desired timer count:
    OCR1A = 7812;
    
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    
    // Set CS10 and CS12 bits for 1024 prescaler:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    
    // enable timer compare interrupt:
    TIMSK |= (1 << OCIE1A);
    
    // enable global interrupts:
    sei();
}

void loop() {

  if(connection_failed_problem>=20)
   {
      digitalWrite(esp_power_supply_pin,HIGH);
      delay(2000);
      digitalWrite(esp_power_supply_pin,LOW);
      hard_restart=0;
      Serial.println("CFP HARD RESTSRT MARA HAI BIDUU");
      connection_failed_problem=0;
      }
  
  delay(10);
  hard_restart++;
  if(hard_restart==3000)
      {
      digitalWrite(esp_power_supply_pin,HIGH);
      delay(2000);
      digitalWrite(esp_power_supply_pin,LOW);
      hard_restart=0;
      Serial.println("HARD RESTSRT MARA HAI BIDUU");
      }
      
  /*Serial.print("clock_hour");
  Serial.println(clock_hour);
  Serial.print("clock_minute");
  Serial.println(clock_minute);
  Serial.print("clock_second");
  Serial.println(clock_second);
   Serial.print("control_time_loop");
  Serial.println(control_time_loop);
   Serial.print("problem_due_to_unix_timestamp");
  Serial.println(problem_due_to_unix_timestamp);*/
  

  if((clock_hour <= 0) && (clock_minute <= 0)&&(control_time_loop==1)&&(problem_due_to_unix_timestamp==0))//39020202 sbi
     {
                                digitalWrite(relay_control_pin,LOW);
                                control_time_loop=0;    
                                Serial.println("TIMER MODE END");
                                //glow_magenta();
                                
                                  
        }
  // print the string when a newline arrives:
  if (stringComplete) {
    hard_restart=0;
    Serial.println(inputString);
    if (inputString.startsWith("ON")) {
      connection_failed_problem=0;
   digitalWrite(led1,HIGH);
   digitalWrite(led2,LOW);
   digitalWrite(led3,HIGH);
   digitalWrite(relay_control_pin,HIGH);
   control_time_loop=0;
  }
  else if (inputString.startsWith("OFF")) {
    connection_failed_problem=0;
    control_time_loop=0;
   digitalWrite(led1,LOW);
   digitalWrite(led2,HIGH);
   digitalWrite(led3,HIGH);
   digitalWrite(relay_control_pin,LOW);
  }
  else if (inputString.startsWith("connection failed")) {
    connection_failed_problem++;
   digitalWrite(led2,HIGH);
   digitalWrite(led3,LOW);
  }
  else if (inputString.startsWith("TIMER")) {
    connection_failed_problem=0;
    problem_due_to_unix_timestamp=1;
    control_time_loop=1;
    sum="";
    inputString.toCharArray(istring,16);
    int i;
    char c;
    
    for(i=0;i<5;i++)
    {
      c=istring[5+i];
      if(c!='J')

        {
          sum+=c;
          Serial.println(sum);
          }
          else
          break;
      
      }
      Serial.println(sum.toInt());
      setTime=sum.toInt();
      clock_hour=setTime/60;
      clock_minute=setTime%60;
      clock_second=0;
      Serial.println(clock_hour);
      Serial.println(clock_minute);
      
      Serial.println("aaja");
    Serial.println(istring[2]);
    Serial.println(istring);
   digitalWrite(relay_control_pin,HIGH);
   digitalWrite(led3,LOW);
   digitalWrite(led2,HIGH);
   digitalWrite(led1,LOW);
   
  }
   
  
problem_due_to_unix_timestamp=0;
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
ISR(TIMER1_COMPA_vect)
{
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
