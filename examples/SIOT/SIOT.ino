/*!
 * @file SIOT.ino
 * @brief In this example, we will use UART or I2C, and IoT MQTT protocol to send "HI DFROBOT 2020" to SIoT.
 * @n SIot is a cross platform open source mqtt server program customized for stem education in primary and secondary schools https://mc.dfrobot.com.cn/thread-281102-1-1.html
 * @n Experimental phenomenon: the sIot website receives messages every 1 second.
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
//Use software serial RX:10,TX:11
SoftwareSerial    mySerial(10, 11);
DFRobot_WiFi_IoT_Module_UART IOT(&mySerial);

//Configure WiFi name and password
const char *WIFI_SSID          = "WIFI_SSID";
const char *WIFI_PASSWORD      = "WIFI_PASSWORD";
//Configure SIot
const char *SIOT_SERVER        = "Your_Address";
const char *SIOT_PORT          = "1883";              
const char *SIOT_ID            = "You_Device_Name";              
const char *SIOT_PWD           = "You_Device_Secret";           
const char *SUBSCRIBE_TOPIC    = "You_Sub_Topic";         
const char *PUBLISH_TOPIC      = "You_Pub_Topic";

const char *SIOT_SEND_MESSAGE  = "Send_Message";      

//Set callback function
void callback(const char*topic,const char*message){
  Serial.println("Receive [ " + (String)topic + "]," + "Message : " + (String)message);
}

void setup(void){
  //Use software serial myserial as communication serial port
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
    delay(500);
  }
  Serial.println("Wifi Connect Success");
  //Initialize MQTT and connect to platform
  while(IOT.MQTTBegin(SIOT_SERVER, SIOT_PORT, SIOT_ID, SIOT_PWD) != 0){
    Serial.print(".");
    delay(500);
  }
  Serial.println("SIOT Connect Success");

  IOT.setCallBack(callback);
  //Subscribe to topics
  while(IOT.subscribe(SUBSCRIBE_TOPIC) != 0){
    Serial.print(".");
    delay(500);
  }
  Serial.println("Subscribe Topic Success");
}

void loop(void){
  //Send data to the subscribed topic.
  if(IOT.publish(PUBLISH_TOPIC, SIOT_SEND_MESSAGE) == 0){
    IOT.loop();
  }else{
    Serial.println("Data sending timeout");
  }
  delay(1000);
}