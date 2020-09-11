/*!
 * @file OneNET.ino
 * @brief Send data flow to the OneNET IoT platform every 1 second, and view the data in the data flow display area.
 * @n Experimental Phenomenon:The data flow display area shows the sent data.
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
const char *WIFI_SSID         = "WIFI_SSID";
const char *WIFI_PASSWORD     = "WIFI_PASSWORD";
//Configure SIot
const char *ONENET_SERVER     = "183.230.40.39";
const char *ONENET_PORT       = "6002";              
const char *ONENET_PRO_ID     = "Your_Prodact_Name";   
const char *ONNNET_DEV_ID     = "Your_Device_Name";
const char *ONENET_PWD        = "Your_Device_Secret";                 
const char *PUBLISH_TOPIC     = "$dp";  
const char *SUBSCRIBE_TOPIC   = "$dp";

const char *SEND_MESSAGE      = "Send_Message" ;
const char *DATA_STREAM_NAME  = "Data_stream_name";


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
  while(IOT.MQTTBegin(ONENET_SERVER, ONENET_PORT, ONENET_PRO_ID, ONENET_PWD ,ONNNET_DEV_ID) != 0){
    Serial.print(".");
    delay(500);
  }
  Serial.println("SIOT Connect SUCCESS");

  while(IOT.subscribe(SUBSCRIBE_TOPIC) != 0){
    Serial.print(".");
    delay(100);
  }
  Serial.println("Subscribe Topic Success");
}

void loop(void){
  //Send data to the subscribed topic.
  if(IOT.publish(PUBLISH_TOPIC,(uint8_t *)tempdata(DATA_STREAM_NAME, SEND_MESSAGE).c_str(),tempdata(DATA_STREAM_NAME, SEND_MESSAGE).length()) == 0){
    Serial.println("Data sending Success");
  }else{
    Serial.println("Data sending timeout");
  }
  delay(1000);
}

String splicingData(String streamname, String data){
  String str = "";
  String str2 = "";
  str2 += "{\"";
  str2 += streamname;
  str2 +="\":";
  str2 += data;
  str2 += "}";
  
  str += (char)0x03;
  str += (char)(str2.length()>>8);
  str += (char)str2.length();
  str += str2;
  return str;
}