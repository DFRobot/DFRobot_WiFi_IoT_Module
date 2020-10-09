/*!
 * @file BeeBotte.ino
 * @brief In this example, we will use UART or I2C, and IoT MQTT protocol to send "Hi, DFRobot" to BeeBottew website.
 * @n Experimental Phenomenon: BeeBotte website receives message every 1 second
 *
 * @copyright    Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence      The MIT License (MIT)
 * @author       [Tang](jie.tang@dfrobot.com)
 * @version      V1.0
 * @date         2020-07-13
 * @get          from https://www.dfrobot.com
 * @url          https://github.com/DFRobot/DFRobot_WiFi_IOT_Module
 */
#include "DFRobot_WiFi_IoT_Module.h"
#include <SoftwareSerial.h>
//I2C mode
//DFRobot_WiFi_IoT_Module_I2C IOT;
//UART mode
//Use softwareserial RX:10,TX:11
SoftwareSerial    mySerial(10, 11);
DFRobot_WiFi_IoT_Module_UART IOT(&mySerial);

//Configure WiFi name and password
const char *WIFI_SSID              = "WIFI_SSID";
const char *WIFI_PASSWORD          = "WIFI_PASSWORD";
//Beebotte configuration
const char *BEEBOTTE_ADDRESS       = "api.beebotte.com";
const char *BEEBOTTE_TOKEN         = "Your_Channel_Token";
const char *BEEBOTTE_CHANNEL       = "Your_Channel_Name";
const char *BEEBOTTE_RESOURCE      = "Your_Resource_Name";

const char *BEEBOTTE_SEND_MESSAGE  = "Send_Message";

void setup(void){
  //Using softwareserial myserial as communication serial port
  mySerial.begin(9600);
  //Using serial as print serial port
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
  //
  while(IOT.HTTPBegin(BEEBOTTE_ADDRESS) != 0){
    Serial.println("Parameter is empty!");
    delay(100);
  }
  Serial.println("HTTP Configuration Succeeded");
  //Initialize beebotte 
  while(IOT.beebotteBegin(BEEBOTTE_TOKEN) != 0){
    Serial.print("Parameter is empty!");
    delay(100);
  }
  Serial.println("Beebotte Configuration Succeeded");
}

void loop(void){
  if(IOT.beebotteSendMessage(BEEBOTTE_CHANNEL, BEEBOTTE_RESOURCE, BEEBOTTE_SEND_MESSAGE) == 0){
      Serial.println("Data sending Success");
  }else{
    Serial.println("Data sending timeout");
  }
  delay(1000);
}