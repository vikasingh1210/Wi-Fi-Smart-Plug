# Wi-Fi-Smart-Plug
This work of mine include work of 
Kevin Darrah (https://www.kevindarrah.com/), 
Pattrick-Hood Daniel(https://www.newbiehack.com/), for embedded code of ESP32. 
Atmega8, android app, server code, hardware design is completely developed by me. No part of them is copied from anywhere, these are completly build from scratch.
Development of Wi-Fi smart plug included these 3 main parts
1. embedded, 
2.server and 
3. android code.
In this project I have developed a Wi-Fi based smart plug which will be place between the power supply socket and power cord of device.
  Using the android app we can configure the plug to our home wi-fi router. Once configured, app will create a controlling page for the plug. We can add as many smart plug to the same app. It will create different controlling pages for each plug. All the plugs configured from same phone will shown on a list which is opening page of app. Click on the device you want to control from the list. It will take you to the page of that device.
  Embedded code will be based on arduino IDE of ESP 8266 and atmega8. Due to unstable ESP 8266 we have use ATmega8 to take final desicion on opening and closing of relay

User need to install the .apk file to his phone

Burn the file by name of "sketch_may24a.ino" to ESP8266 using FTDI 

Burn the file by name of "Atmega8_CODE.ino" to atmega8 using arduino as ISP 

Rest all file should be install on server using cPanel. I have used godaddy
