/*!
 * @file Easy_IoT.ino
 * @brief  In this example, we will use UART or I2C, and MQTT protocol to send "HI DFROBOT 2020" to Easy IoT website
 * @n Experimental Phenomenon: easy_IoT website receives message every 1 second.
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
//Use software serial port RX:10,TX:11
SoftwareSerial    mySerial(10, 11);
DFRobot_WiFi_IoT_Module_UART IOT(&mySerial);

//Configure WiFi name and password
const char *WIFI_SSID                = "WIFI_SSID";
const char *WIFI_PASSWORD            = "WIFI_PASSWORD";
//Easy IOT EN configuration
const char *EASY_IOT_SERVER          = "iot.dfrobot.com.cn";
//Easy IOT EN configuration
//const char *EASY_IOT_SERVER        = "iot.dfrobot.com";
const char *EASY_IOT_PORT            = "1883";
const char *EASY_IOT_ID              = "Your_Device_Name";
const char *EASY_IOT_PWD             = "Your_Device_Secret";
const char *SUBSCRIBE_TOPIC          = "Your_Sub_Topic";
const char *PUBLISH_TOPIC            = "Your_Pub_Topic";
const char *EASY_IOT_SEND_MESSAGE    = "Send_Message";

//Set callback function
void callback(const char*topic,const char*message){
  Serial.println("Receive [ " + (String)topic + "]," + "Message : " + (String)message);
}

void setup(void){
  //Use softwareserial myserial as communication serial port
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
  //Initialize MQTT and connect to platform
  while(IOT.MQTTBegin(EASY_IOT_SERVER, EASY_IOT_PORT, EASY_IOT_ID, EASY_IOT_PWD) != 0){
    Serial.print(".");
    delay(100);
  }
  Serial.println("MQTT Connect Success");
  //Set callback function
  IOT.setCallBack(callback);
  //Subscribe to topics
  while(IOT.subscribe(SUBSCRIBE_TOPIC) != 0){
    Serial.print(".");
    delay(100);
  }
  Serial.println("Subscribe Topic Success");
}

void loop(void){
  //Send data to the subscribed topic.
  if(IOT.publish(PUBLISH_TOPIC,EASY_IOT_SEND_MESSAGE) == 0){
      IOT.loop();
  }else{
    Serial.println("Data sending timeout");
  }
  delay(1000);
}