/*!
 * @file ThingSpeak.ino
 * @brief In this example, we will use UART or I2C, and HTTP protocol to send data to ThingSpeak website.
 * @n Experimental Phenomenon: ThingSpeak receives data every 1 second 
 *
 * @copyright    Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence      The MIT License (MIT)
 * @author       [Tang](jie.tang@dfrobot.com)
 * @version      V1.0
 * @date         2020-07-13
 * @get         from https://www.dfrobot.com
 * @url         https://github.com/DFRobot/DFRobot_WiFi_IOT_Module
 */
#include "DFRobot_WiFi_IoT_Module.h"
#include <SoftwareSerial.h>
//I2C mode
//DFRobot_WiFi_IoT_Module_I2C IOT;
//UART mode
//Use software serial RX：10，TX：11
SoftwareSerial    mySerial(10, 11);
DFRobot_WiFi_IoT_Module_UART IOT(&mySerial);

//Configure WiFi name and password
const char *WIFI_SSID             = "WIFI_SSID";
const char *WIFI_PASSWORD         = "WIFI_PASSWORD";
//Configure Thingspeak
const char *ThingSpeak_ADDRESS    = "api.thingspeak.com";
const char *THINGSPEAK_KEY        = "Your_key";
//Thingspeak Send Message
const char *THINGSPEAK_VALUE_1    = "Value1";
const char *THINGSPEAK_VALUE_2    = "Value2";
const char *THINGSPEAK_VALUE_3    = "Value3";

void setup() {
  //Use software-serial myserial as communication serial port
  mySerial.begin(9600);
  //Use serial as print serial port
  Serial.begin(115200);
  //Init communication port
  while(IOT.begin() != 0){  
    Serial.println("init ERROR!!!!");
    delay(100);
  }
  Serial.println("init Success");
  //Connect WiFi
  while(IOT.connectWifi(WIFI_SSID, WIFI_PASSWORD) != 0){  
    Serial.print(".");
    delay(100);
  }
  Serial.println("Wifi Connect Success");

  while(IOT.HTTPBegin(ThingSpeak_ADDRESS) != 0){
    Serial.println("Parameter is empty!");
    delay(100);
  }
  Serial.println("HTTP Configuration Success");
  
  while(IOT.thingSpeakBegin(THINGSPEAK_KEY) != 0){
    Serial.print("Parameter is empty!");
    delay(100);
  }
  Serial.println("ThingSpeak Configuration Success");
}

void loop() {
  if(IOT.thingSpeakSendMessage(THINGSPEAK_VALUE_1, THINGSPEAK_VALUE_2, THINGSPEAK_VALUE_3) == 0){
    Serial.println("Data sending Success");
  }else{
    Serial.println("Data sending timeout");
  }
  delay(1000);
}