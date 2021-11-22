#include <DFRobot_WiFi_IoT_Module.h>
#include <string.h>

uint8_t DFRobot_WiFi_IoT_Module_I2C::begin(void)
{
  uint8_t buffer[2];
  _pWire->begin();
  if(readReg(0,buffer, 2) == 2 ){

    clearBuffer();
    DBG("WIFI_IOT_OK");
    return WIFI_IOT_OK;
  }
  DBG("WIFI_IOT_ERROR!!!");
  return WIFI_IOT_ERROR;
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::connectWifi(char *ssid, char *pwd)
{
  if(_wifiSSID != ssid){
    DBG("WIFI");
    _wifiSSID = ssid;
    manageFunction(IOT_SET_COMMAND, SET_WIFI_NAME, ssid);
    manageFunction(IOT_SET_COMMAND, SET_WIFI_PASSWORLD, pwd);
    connection(CONNECT_WIFI);
  }
  return getWifiState();
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::MQTTBegin(char *server, char *port, char *productID, char *pwd, char* deviceID = NULL)
{
  if(_mqttproductID != productID || _mqttPwd != pwd || _mqttServer != server || _mqttPort != port ){
    DBG("MQTT");
    _mqttproductID  = productID;
    _mqttPwd        = pwd;
    _mqttServer     = server;
    _mqttPort       = port; 
    if(deviceID == NULL){
      manageFunction(IOT_SET_COMMAND, SET_MQTT_SERVER, server);
      manageFunction(IOT_SET_COMMAND, SET_MQTT_PORT, port);
      manageFunction(IOT_SET_COMMAND, SET_MQTT_ID, productID);
      manageFunction(IOT_SET_COMMAND, SET_MQTT_PASSWORLD, pwd);
      connection(CONNECT_MQTT);
    }else{
      manageFunction(IOT_SET_COMMAND, SET_MQTT_SERVER, server);
      manageFunction(IOT_SET_COMMAND, SET_MQTT_PORT, port);
      manageFunction(IOT_SET_COMMAND, SET_MQTT_ID, productID);
      manageFunction(IOT_SET_COMMAND, SET_MQTT_PASSWORLD, pwd);
      manageFunction(IOT_SET_COMMAND, SET_MQTT_DEVICEID, deviceID);
      connection(CONNECT_MQTT_ONENET);
    }
  }
  return getMQTTState();
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::subscribe(char *topic)
{
  char *_topic = topic;
  static uint8_t num = 0, subscribeTopic = 0;
  if(num>5){
    return FULL_SUBSCRIPTINO;
  }
  if(_MQTTTopic != _topic){
    _MQTTTopic = _topic;
    for(uint8_t i = 0; i < 5; i++){
      if(strcmp(_topicName[i],_topic) == 0){
        return REPEAT_SUBSCRIPTION;
      }
    }
    _topicName[num] = _topic;
    DBG(_topicName[num]);
    switch(num){
    case TOPIC0:
      subscribeTopic = 0x06;
      num++;
      break;
    case TOPIC1:
      subscribeTopic = 0x07;
      num++;
      break;
    case TOPIC2:
      subscribeTopic = 0x08;
      num++;
      break;
    case TOPIC3:
      subscribeTopic = 0x09;
      num++;
      break;
    case TOPIC4:
      subscribeTopic = 0x0A;
      num++;
      break;
    default:
      return RETURN_NONE;
    }
    manageFunction(IOT_RUN_COMMAND, subscribeTopic, _topic);//Subscribe to topic
  }
  if(strncmp(topic,"$dp",3) ==0){
    return 0;
  }
  return getTopicState();
}

void DFRobot_WiFi_IoT_Module_I2C::clearBuffer(void)
{
  uint8_t buf[10];
  buf[0] = 0x02;
  buf[1] = 0x17;
  writeReg(0x1E, buf, (uint8_t)2);
  //Serial.println("clear buffer!");
  delay(1000);
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::publish(char *topic, String data)
{
  uint8_t ret = -1;
  char *_topic = topic;
  if((!data) || (!_topic)){
    ret = 3;
  }else{
    uint8_t num =254, sendTopic = 0;
    for(uint8_t i = 0; i < 5;i++){
      if(strcmp(_topicName[i],_topic) == 0){
        num = i;
        break;
      }
    }
    DBG(num);
    switch(num){
    case TOPIC0:
      sendTopic = 0x0B;
      break;
    case TOPIC1:
      sendTopic = 0x0C;
      break;
    case TOPIC2:
      sendTopic = 0x0D;
      break;
    case TOPIC3:
      sendTopic = 0x0E;
      break;
    case TOPIC4:
      sendTopic = 0x0F;
      break;
    default:
      ret = 1;
    }
    manageFunction(IOT_RUN_COMMAND, sendTopic, data);
  }
  uint32_t startingTime = millis();
  while(1){
    uint32_t currentTime = millis();
    delay(100);
    if(getMQTTSendState() == 1){
      ret = 0;
      break;
    }else if((currentTime-startingTime) > 100000){
      ret = -1;
      break;
    }
  }
  return ret;
}
uint8_t DFRobot_WiFi_IoT_Module_I2C::publish(char *topic, uint8_t *data, uint16_t len)
{
  uint8_t ret = -1;
  char *_topic = topic;
  if((!data) || (!_topic)){
    ret = 3;
  }else{
    uint8_t num =254, sendTopic = 0;
    for(uint8_t i = 0; i < 5;i++){
      if(strcmp(_topicName[i],_topic) == 0){
        num = i;
        break;
      }
    }
    DBG(num);
    switch(num){
    case TOPIC0:
      sendTopic = 0x0B;
      break;
    case TOPIC1:
      sendTopic = 0x0C;
      break;
    case TOPIC2:
      sendTopic = 0x0D;
      break;
    case TOPIC3:
      sendTopic = 0x0E;
      break;
    case TOPIC4:
      sendTopic = 0x0F;
      break;
    default:
      ret = 1;
    }
  manageFunction(IOT_RUN_COMMAND, sendTopic, data, len);
  }
  uint32_t startingTime = millis();
  while(1){
    uint32_t currentTime = millis();
    delay(100);
    if(getMQTTSendState() == 1){
      ret = 0;
      break;
    }else if((currentTime-startingTime) > 100000){
      ret = -1;
      break;
    }
    
  }
  return ret;
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::HTTPBegin(char *ip)
{
  if((!ip)){
    return -1;
  }else{
    manageFunction(IOT_SET_COMMAND, SET_HTTP_ID, ip);
  }
  return 0;
}

uint8_t DFRobot_WiFi_IoT_Module_UART::HTTPBegin(char *ip)
{
   if( _httpip != ip){
      _httpip = ip;
      return 0;
    }
    return -1;
}

String DFRobot_WiFi_IoT_Module_I2C::HTTPGet(char *url)
{
  uint8_t recvHTTPData[25];
  manageFunction(IOT_RUN_COMMAND, HTTP_GET_URL, url);
  uint32_t startingTime = millis();
    uint8_t state = parameterReturn(HTTP_NORMAL_RETURN, HTTP_ERROR_RETURN, &recvHTTPData[0]);
    
    if(state == 0){
      return (char*)recvHTTPData;
    }else if(state == 1){
      return "timeout";
    }
    
      

}

String DFRobot_WiFi_IoT_Module_UART::HTTPGet(char *url)
{
  String comdata = "";
  String httpGetMag = "";
  httpGetMag +="|3|1|http://";
  httpGetMag +=_httpip;
  httpGetMag +="/";
  httpGetMag +=url;
  httpGetMag +=_separator;
  httpGetMag += "\r";
  DBG(httpGetMag);
  //Serial.print(httpGetMag);
  _s->print(httpGetMag);


  if(receiveData() == 0){
    return _receiveStringIndex[httpProtocol::httpMessage];
  }else{
    return "error";
  }
  /*
  uint32_t startingTime = millis();
  while(true){
    if(receiveData() != 0){
      return "read error";
    }
    if(_receiveStringIndex[httpProtocol::httpType] == HTTPTYPE){
      if(_receiveStringIndex[httpProtocol::httpCode] == "200"){
        return _receiveStringIndex[httpProtocol::httpMessage];
      }
    }
    delay(100);
    uint32_t currentTime = millis();
    if((currentTime-startingTime) > 10000){
      return "timeout";
    }
  }
  */
}

String DFRobot_WiFi_IoT_Module_I2C::HTTPPost(char* postUrl, char* data)
{
  uint8_t recvHTTPData[25];
  String url = "";
  url += postUrl;
  url += ',';
  url += data;
  DBG(url);
  manageFunction(IOT_RUN_COMMAND, HTTP_POST_URL_CON, (uint8_t*)url.c_str());
  uint32_t startingTime = millis();
    uint8_t state = parameterReturn(HTTP_NORMAL_RETURN, HTTP_ERROR_RETURN, &recvHTTPData[0]);
    if(state == 0){
      return (char*)recvHTTPData;
    }else if(state == 1){
      return "timeout";
    }
}

String DFRobot_WiFi_IoT_Module_UART::HTTPPost(char* postUrl, char* data)
{
  String httpPostMag ="";
    httpPostMag += "|3|2|http://";
    httpPostMag += _httpip;
    httpPostMag += "/";
    httpPostMag += postUrl;
    httpPostMag += ",";
    httpPostMag += data;
    httpPostMag += _separator;
    httpPostMag += "\r";
    DBG(httpPostMag);
    //Serial.println(httpPostMag);
    _s->print(httpPostMag);
    if(receiveData() == 0){
      return _receiveStringIndex[httpProtocol::httpMessage];
    }else{
      return "error";
    }
    /*
  while(true){
    
    uint32_t startingTime = millis();
    if(receiveData() != 0){
      return "error";
    }
    if(_receiveStringIndex[httpProtocol::httpType] == HTTPTYPE){
      if(_receiveStringIndex[httpProtocol::httpCode] == "200"){
        return _receiveStringIndex[httpProtocol::httpMessage];
      }
    }
    delay(100);
    uint32_t currentTime = millis();
    if((currentTime-startingTime) > 100000){
      return "error";
    }
    
  }
  */
}

void DFRobot_WiFi_IoT_Module_I2C::manageFunction(uint8_t command, uint8_t config, String data)
{
  uint8_t i = 0, j = 0;
  i = data.length();
  uint8_t datalen = i+3;
  uint8_t *buffer = (uint8_t *)malloc(sizeof(uint8_t)*datalen);
  if(buffer == NULL){
    Serial.print("memory allocation failed");
    return;
  }
  buffer[0]  =  command;
  buffer[1]  =  config;
  buffer[2]  =  datalen-3;
  for(i = 3,j = 0; i < datalen; i++){
    buffer[i] = (char)data[j++];
  } 
  writeReg(IOT_COMMAND_REGTSTER, buffer, datalen);
  free(buffer);
}
void DFRobot_WiFi_IoT_Module_I2C::manageFunction(uint8_t command, uint8_t config, uint8_t *data, uint16_t len)
{
  uint8_t i = 0, j = 0;
  uint8_t datalen = len+3;
  uint8_t *buffer = (uint8_t *)malloc(sizeof(uint8_t)*datalen);
  if(buffer == NULL){
    Serial.print("memory allocation failed");
    return;
  }
  buffer[0]  =  command;
  buffer[1]  =  config;
  buffer[2]  =  len;
  for(i = 3,j = 0; i < datalen; i++,j++){
    buffer[i] = data[j];
  } 

  writeReg(IOT_COMMAND_REGTSTER, buffer, datalen);
  free(buffer);
}

void DFRobot_WiFi_IoT_Module_I2C::reconnectWifi()
{
  connection(RECONNECT_WIFI);
  while(1){
    if(getWifiState() == 1){
      break;
    }
  }
  _wifiState = MQTT_SUCCESS_CON;
}

bool DFRobot_WiFi_IoT_Module_I2C::disconnectWifi()
{
  connection(BREAK_WIFI_CON);
  while(1){
    if(getWifiState() == 1){
      break;
    }
  }
  _wifiState = WIFI_NO_CON;
  return true;
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::getWifiState()
{
  uint8_t buffer[2]={0};
  if(readReg(IOT_COMMAND_REGTSTER, &buffer, 2) != 2){
    DBG("READ WIFI_IOT_ERROR!!!!!!");
    return WIFI_IOT_ERROR;
  }else{
    if(buffer[0] == WIFI_STATUE_CON){
      if(buffer[1] == WIFI_SUCCESS_CON){
        DBG("WIFI SUCCESS CON");
        _wifiState = WIFI_SUCCESS_CON;            //WiFi connected
        return 0;
      }else if(buffer[1] == WIFI_NO_CON){
        DBG("WIFI NO CON");
        _wifiState = WIFI_NO_CON;                 //Wifi connection error
        return 1;
      }else if(buffer[1] == WIFI_RUN_CON){
        DBG("WIFI RUN CON");
        _wifiState = WIFI_RUN_CON;               //Connecting to WiFi
        return 2;
      }else{
        DBG("WIFI WIFI_IOT_ERROR");
        return WIFI_IOT_ERROR;
      }
    }else{
      DBG("NULL");
      return WIFI_IOT_ERROR;
    }
  }
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::getMQTTState()
{
  uint8_t buffer[2];
  if(readReg(IOT_COMMAND_REGTSTER, &buffer, 2) != 2){
    DBG("READ WIFI_IOT_ERROR!!!!!!");
    return WIFI_IOT_ERROR;
  }else{
    DBG(buffer[0]);
    if(buffer[0] == MQTT_STATUE_CON){
      if(buffer[1] == MQTT_SUCCESS_CON){
        DBG("MQTT_SUCCESS_CON");
        return 0;
      }else if(buffer[1] == MQTT_ERROR_CON){
        DBG("MQTT_ERROR_CON");
        return 1;
      }else if(buffer[1] == MQTT_START){
        DBG("MQTT_START");
        return 2;
      }
    }else if(buffer[0] == NONE){
      DBG("RETURN_NONE");
      return RETURN_NONE;
    }else{
      DBG("WIFI_IOT_ERROR");
      return WIFI_IOT_ERROR;
    }
  }
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::getMQTTSendState()
{
  uint8_t buffer[2];
  if(readReg(IOT_COMMAND_REGTSTER, &buffer, 2) != 2){
    DBG("READ WIFI_IOT_ERROR!!!!!!");
    return WIFI_IOT_ERROR;
  }else{
    DBG(buffer[0]);
    if(buffer[0] == GET_MQTTSEND_STATUE){
      if(buffer[1] == MQTT_SEND_SUCCESS){
        DBG("MQTT_SEND_SUCCESS");
        return 1;
      }else if(buffer[1] == MQTT_SEND_ERROR){
        DBG("MQTT_SEND_ERROR");
        return 2;
      }else{
        DBG("ERROR");
        return -1;
      }
    }
  }
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::getTopicState()
{
  uint8_t buffer[2] ;
  if(readReg(IOT_COMMAND_REGTSTER, &buffer, 2) != 2){
    DBG("READ WIFI_IOT_ERROR!!!!!!");
    return WIFI_IOT_ERROR;
  }else{
    if(buffer[0] == SUBSCRIBE_STATUE){
      switch(buffer[1]){
      case TAKE_SUCCESS:
        return 0;
        break;
      case TAKE_ERROR:
        return 1;
        break;
      case TAKE_UPPER_LIMIT:
        return 2;
        break;
      default:
        return 3;
      }
    }else if(buffer[0] == NONE){
      return RETURN_NONE;
    }else{
      return WIFI_IOT_ERROR;
    }
  }
}

void DFRobot_WiFi_IoT_Module_I2C::connection(uint8_t command)
{
  uint8_t buffer[2];
  buffer[0] = 0x02;
  buffer[1] = command;
  writeReg(IOT_COMMAND_REGTSTER, &buffer, 2);	
}

String DFRobot_WiFi_IoT_Module_I2C::getVersion()
{
  uint8_t getVersionData[5];
  connection(QUERY_VERSION);
  delay(10);
  uint8_t state = parameterReturn(GET_VERSION, 0x00, &getVersionData[0]);
  if(state != 0){
    DBG("Parameter return failed");
    return;
  }
  return (char*)getVersionData;
}

void DFRobot_WiFi_IoT_Module_I2C::setCallBack(callback call)
{
  this->_callback = call;
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::loop()
{
  uint8_t i = 0, topic;
  uint8_t buffer[3];
  if(readReg(IOT_COMMAND_REGTSTER, &buffer, 3) != 3){
    DBG("READ WIFI_IOT_ERROR!!!!!!");
    return WIFI_IOT_ERROR;
  }else{
    if(buffer[0] == PING_STATUE){
    }else if(buffer[0] == WIFI_STATUE_CON){
      _wifiState = buffer[2];
    }else if(buffer[0] == RECV_TOPIC0){
      topic = 0;
    }else if(buffer[0] == RECV_TOPIC1){
      topic = 1;
    }else if(buffer[0] == RECV_TOPIC2){
     topic = 2;
    }else if(buffer[0] == RECV_TOPIC3){
      topic = 3;
    }else if(buffer[0] == RECV_TOPIC4){
      topic = 4;
    }else{
      return;
    }
    uint8_t datalen = buffer[1];
    if(datalen > 0x7F){
      return;
    }
    uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t)*datalen);
    if(data == NULL){
    Serial.print("memory allocation failed");
    return;
    }
    if(_callback){
      if(getData(READ_DATA_REGISTER, data,datalen) != datalen){
        DBG("READ WIFI_IOT_ERROR!!!!!!");
        return WIFI_IOT_ERROR;
      }else{
        _callback(_topicName[topic], data);
        free(data);
      }
    }
  }
  return 0;
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::IFTTTSendMessage(char *data1, char *data2, char *data3)
{
  uint8_t recvHTTPData[100];
  String sendData = ""; 
  sendData += "{\"value1\":\"";
  sendData += data1;
  sendData += "\",\"value2\":\"";
  sendData += data2;
  sendData += "\",\"value3\":\"";
  sendData += data3;
  sendData += "\" }\r";
  String url = "";
  url += "trigger/";
  url += _iftttevent;
  url += "/with/key/";
  url += _iftttkey;
  
  String ur2 = "";
  ur2 += url;
  ur2 += ',';
  ur2 += sendData;
  DBG(url);
  manageFunction(IOT_RUN_COMMAND,HTTP_POST_URL_CON,(char*)ur2.c_str(),ur2.length());
  uint8_t state = parameterReturn(HTTP_NORMAL_RETURN, HTTP_ERROR_RETURN, &recvHTTPData[0]);
  if(state == 0){
    return 0;
  }
  return 1;
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::thingSpeakSendMessage(char* data1, char *data2, char *data3)
{
  String sendData = "";
  sendData += "update?api_key=";
  sendData += _thingspeeakkey;
  sendData += "&field1=";
  sendData += data1;
  sendData += "&field2=";
  sendData += data2;
  sendData += "&field3=";
  sendData += data3;
  uint8_t recvHTTPData[100];
  manageFunction(IOT_RUN_COMMAND, HTTP_GET_URL, sendData);
  uint32_t startingTime = millis();
    uint8_t state = parameterReturn(HTTP_NORMAL_RETURN, HTTP_ERROR_RETURN, &recvHTTPData[0]);
    if(state == 0){
    return 0;
    }
  return 1;
}

void DFRobot_WiFi_IoT_Module_I2C::writeReg(uint8_t reg, void *pBuf, size_t size)
{
  uint8_t *_pBuf = (uint8_t*)pBuf;
  _pWire->beginTransmission(_address);
  _pWire->write(reg);
  if(size > 32){
    uint16_t j = 1;
    for(uint8_t i = 0; i<size; i++){
      if(i >= (31*j)){
        _pWire->endTransmission(false);
        _pWire->beginTransmission(_address); 
        j++;
      }
      _pWire->write(_pBuf[i]);
    }
  }else{
    DBG("D");
    for(size_t i = 0; i < size; i++){
      _pWire->write(_pBuf[i]);
    }
  }
  _pWire->endTransmission();
  delay(20);
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::readReg(uint8_t reg, void *pBuf, size_t size){
  if(pBuf == NULL){
    DBG("pBuf WIFI_IOT_ERROR!!!!");
    return 0;
  }
  uint8_t *_pBuf = (uint8_t*)pBuf;
  _pWire->beginTransmission(_address);
  _pWire->write(reg);
  _pWire->write(IOT_READ_COMMAND);
  _pWire->write(READ_COMMAND);
  if(_pWire->endTransmission() != 0){
    return 0;
  }
  delay(100);
  _pWire->requestFrom(_address,size);
  for(size_t i = 0; i < size; i++){
    _pBuf[i] = _pWire->read();
  }
  return size;
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::getData(uint8_t reg, void *pBuf, size_t size)
{
  if(pBuf == NULL){
    DBG("pBuf WIFI_IOT_ERROR!!!!");
    return 0;
  }
  uint8_t *_pBuf = (uint8_t*)pBuf;
  _pWire->beginTransmission(_address);
  _pWire->write(reg);
  if(_pWire->endTransmission() != 0){
    return 0;
  }
  _pWire->requestFrom(_address,size);
  for(size_t i = 0; i < size; i++){
    _pBuf[i] = _pWire->read();
  }
  return size;
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::parameterReturn(uint8_t config, uint8_t config1, uint8_t *pBuf)
{
  uint8_t buffer[2]={0};
  uint32_t startingTime = millis();
  while(1){
    readReg(IOT_COMMAND_REGTSTER, &buffer, 2);
    if(buffer[0] == config && buffer[1] < 0xFE){
      uint8_t datalen = buffer[1];
      uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t)*datalen+10);
      if(data == NULL){
        Serial.print("memory allocation failed");
      }
      if(getData(READ_DATA_REGISTER, data, datalen) != datalen){
        DBG("READ WIFI_IOT_ERROR!!!!!!");
        free(data);
      }else{
        for(uint8_t i =0; i < datalen ; i++){
          pBuf[i] = data[i];
        }
        pBuf[datalen]= '\0';
        DBG("true");
        free(data);
        return 0;
      }
    }else if(buffer[0] == config1 && buffer[1] < 0xFE){
      return 2;
    }
    if((millis() - startingTime) > 15000){
      return 1;
    }
  }
  

  
  #if 0
  if(readReg(IOT_COMMAND_REGTSTER, &buffer, 2) != 2){
    DBG("READ WIFI_IOT_ERROR!!!!!!");
  }else{
    DBG(buffer[0]);
    if(buffer[0] == config && buffer[1] < 0xFE){
      
      uint8_t datalen = buffer[1];
      uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t)*datalen+10);
      if(data == NULL){
        Serial.print("memory allocation failed");
      }
      if(getData(READ_DATA_REGISTER, data, datalen) != datalen){
        DBG("READ WIFI_IOT_ERROR!!!!!!");
        free(data);
      }else{
        for(uint8_t i =0; i < datalen ; i++){
          pBuf[i] = data[i];
        }
        pBuf[datalen]= '\0';
        DBG("true");
        free(data);
        return 0;
      }
    }else if(buffer[0] == config1){
      DBG("B");
      pBuf[0] = buffer[1];
      
    }else{
      return 1;
    }
  }
  #endif
  return 1;
}

uint8_t DFRobot_WiFi_IoT_Module_UART::begin(void)
{
  _s->print("|1|1|\r");
  if(receiveData() != 0){
    return -1;
  }
  if(_receiveStringIndex[systemProtocol::systemType] == SYSTEMTYPE){//Init communication port
    DBG("init");
    if(_receiveStringIndex[systemProtocol::systemCode] == SYSTEMPING){//Communication succeed
      return 0;
      DBG("0");
    }
  } 
  return -1;
}

uint8_t DFRobot_WiFi_IoT_Module_UART::connectWifi(char *ssid , char *pwd )
{
  if(_wifiSSID != ssid){
    _wifiSSID = ssid;
    String connectWifiMsg = "";
    connectWifiMsg += "|2|1|";
    connectWifiMsg += _wifiSSID;
    connectWifiMsg += ",";
    connectWifiMsg += pwd;
    connectWifiMsg += _separator;
    connectWifiMsg += "\r";
    _s->print(connectWifiMsg);
  }
  if(receiveData() != 0){
    return -1;
  }
  if(_receiveStringIndex[wifiProtocol::wifiType] == WIFITYPE){//Connect Wifi
    DBG("WIFI");
    if(_receiveStringIndex[wifiProtocol::wifiCode] == WIFIDISCONNECT && this->_wifiState == WIFICONNECTED){
      _wifiState = WIFIDISCONNECT;
      DBG("0");
    }else if(_receiveStringIndex[wifiProtocol::wifiCode] == WIFICONNECTED){
      DBG("0");
      return 0;//WiFi connected    
    }else if(_receiveStringIndex[wifiProtocol::wifiCode] == WIFICONNECTING){
      DBG("1");
      return 1;//Connecting to wifi
      }
    }
    return -1;
}

uint8_t DFRobot_WiFi_IoT_Module_UART::MQTTBegin(char *server = NULL, char *port = NULL, char *productID  = NULL, char *pwd = NULL, char* deviceID = NULL)
{
  if(_mqttState){
    String connectMsg = "";
    if(deviceID==NULL){
      connectMsg += "|4|1|1|";
      connectMsg += server;
      connectMsg += _separator;
      connectMsg += port;
      connectMsg += _separator;
      connectMsg += productID;
      connectMsg += _separator;
      connectMsg += pwd;
      connectMsg += _separator;
      connectMsg += "\r";
    }else{
      connectMsg += "|4|1|1|";
      connectMsg += server;
      connectMsg += _separator;
      connectMsg += port;
      connectMsg += _separator;
      connectMsg += productID;
      connectMsg += _separator;
      connectMsg += pwd;
      connectMsg += _separator;
      connectMsg += deviceID;
      connectMsg += _separator;
      connectMsg += "\r";
    }
    _s->print(connectMsg);
    _mqttState = false;
  }else{
    String reconnectMsg = "|4|1|5|";
    _s->print(reconnectMsg);
  }
  if(receiveData() != 0){
    return -1;
  }
  if(_receiveStringIndex[mqttProtocol::mqttType] == MQTTTYPE){//MQTT
    DBG("MQTT");
    if(_receiveStringIndex[mqttProtocol::mqttFunction] == MQTTCONNECT){//Connect MQTT
      if(_receiveStringIndex[mqttProtocol::mqttCode] == FAILED){
        DBG("1");
        return 1;
      }else{
        DBG(0);
        return 0;
      }
    }
    return -1;
  }
  return -1;
}

uint8_t DFRobot_WiFi_IoT_Module_UART::subscribe(char *topic = NULL)
{
  if(_topicCount >= MAXTOPICNUMBER){//Check whether the subscription is full
    return FULL_SUBSCRIPTINO;
  }
  for(uint8_t i = 0; i < _topicCount; i++){//Detect duplicate subscription
    if(strcmp(_topicArray[i],topic) == 0){
      return REPEAT_SUBSCRIPTION;
    }
  }
  String subscribeMsg =  "";
  subscribeMsg +=  "|4|1|2|";
  subscribeMsg +=  topic;
  subscribeMsg +=  _separator;
  subscribeMsg += "\r";
  _s->print(subscribeMsg);
  if(receiveData() != 0){
    return -1;
  }
  if(_receiveStringIndex[mqttProtocol::mqttType] == MQTTTYPE){
    if(_receiveStringIndex[mqttProtocol::mqttFunction] == MQTTSUBSCRIBE){//Mqtt subscription
      DBG("TOPIC");
      if(_receiveStringIndex[mqttProtocol::mqttCode] == FAILED){//Subscription failed
        _enable = false;
        DBG("1");
        return 3;
      }else{//Subscription succeed
        _enable = true;
        DBG("2");
        _topicArray[_topicCount++] = topic;
        return 0;
      }
    }
  }
}

String DFRobot_WiFi_IoT_Module_UART::getVersion(void)
{
  _s->print("|1|2|\r");
  if(receiveData() != 0){
    return;
  }
  if(_receiveStringIndex[systemProtocol::systemType] == SYSTEMTYPE){
    DBG("init");
    if(_receiveStringIndex[systemProtocol::systemCode] == SYSTEMVERSION){// Get Version number
      _firmwareVersion = _receiveStringIndex[systemProtocol::systemMessage];
    }
  } 
  return _firmwareVersion;
}

uint8_t DFRobot_WiFi_IoT_Module_UART::publish(char *topic, String data)
{
   
  String subscribeMsg = "";
    subscribeMsg += "|4|1|3|";
    subscribeMsg += topic;
    subscribeMsg += _separator;
    subscribeMsg += data;
    subscribeMsg += _separator;
    subscribeMsg += "\r";
  _s->print(subscribeMsg);
  uint32_t startingTime = millis();
  while(true){
    if(receiveData() != 0){
      return -1;
    }
    if(_receiveStringIndex[mqttProtocol::mqttType] == MQTTTYPE){
      if(_receiveStringIndex[mqttProtocol::mqttFunction] == MQTTPUBLISH){
        if(_receiveStringIndex[mqttProtocol::mqttCode] == MQTTCONNECT){
          DBG("publish ok");
          return 0;
        }
      } 
    }
    uint32_t currentTime = millis();
    if((currentTime-startingTime) > 100000){
      return -1;
    }
  }
  return -1;
}

uint8_t DFRobot_WiFi_IoT_Module_UART::publish(char *topic, uint8_t *data, uint16_t len)
{
  char* str = "";
  String subscribeMsg = "";
    subscribeMsg += "|4|1|3|";
    subscribeMsg += topic;
    subscribeMsg += _separator;
    _s->print(subscribeMsg);
    for(uint8_t i =0; i<len; i++){
      _s->print((char)data[i]);
    }
    _s->print(_separator);
    _s->print("\r");
  uint32_t startingTime = millis();
  while(true){
    if(receiveData() != 0){
      return -1;
    }
    if(_receiveStringIndex[mqttProtocol::mqttType] == MQTTTYPE){
      if(_receiveStringIndex[mqttProtocol::mqttFunction] == MQTTPUBLISH){
        if(_receiveStringIndex[mqttProtocol::mqttCode] == MQTTCONNECT){
          DBG("publish ok");
          return 0;
        }
      } 
    }
    uint32_t currentTime = millis();
    if((currentTime-startingTime) > 100000){
      return -1;
    }
  }
  return -1;
}

uint8_t DFRobot_WiFi_IoT_Module_UART::loop(void)
{
  uint32_t startingTime = millis();
  while(true){
    if(receiveData() != 0){
      return -1;
    }
    if(_receiveStringIndex[mqttProtocol::mqttType] == MQTTTYPE){
      if(_receiveStringIndex[mqttProtocol::mqttFunction] == MQTTRECEIVEMSG){
        _enable = true;
        DBG("MQTTRECEIVEMSG");
        if(this->_callback){
          DBG("_callback");
          this->_callback((char*)_receiveStringIndex[mqttProtocol::mqttTopic].c_str(),(char*)_receiveStringIndex[mqttProtocol::mqttMessage].c_str());
          return 0;
        }
      }
    }else if(_receiveStringIndex[httpProtocol::httpType] == HTTPTYPE){
      if(_receiveStringIndex[httpProtocol::httpCode] == "200"){
        DBG("0");
        if(this->_callback){
          this->_callback((char*)_receiveStringIndex[httpProtocol::httpCode].c_str(),(char*)_receiveStringIndex[httpProtocol::httpMessage].c_str());
          return 0;
        }
      }else{
        DBG("1");
        if(this->_callback){
          this->_callback((char*)_receiveStringIndex[httpProtocol::httpCode].c_str(),(char*)_receiveStringIndex[httpProtocol::httpMessage].c_str());
          return 0;
        }
      }
    }
    uint32_t currentTime = millis();
    if((currentTime-startingTime) > 100000){
      return -1;
    }
  }
}

uint8_t DFRobot_WiFi_IoT_Module_UART::IFTTTSendMessage(char *data1, char *data2, char *data3)
{
  String sendData = "";
  sendData += "{\"value1\":\"";
  sendData += data1;
  sendData += "\",\"value2\":\"";
  sendData += data2;
  sendData += "\",\"value3\":\"";
  sendData += data3;
  sendData += "\"}";
  String url = "";
  url += "trigger/";
  url +=_iftttevent;
  url +=  "/with/key/";
  url += _iftttkey;
  String test = HTTPPost((char* )url.c_str(), (char*)sendData.c_str());
  if(strcmp((const char*)test.c_str() ,"error") == 0){
    return -1;
  }else{
    return 0;
  }
}

uint8_t DFRobot_WiFi_IoT_Module::IFTTTBegin(char *event, char *key)
{
  if((!event)||(!key)){
    return -1;
  }
  _iftttevent = event;
  _iftttkey = key;
  return 0;
}

uint8_t DFRobot_WiFi_IoT_Module::thingSpeakBegin(char *key)
{
  if(!key){
    return -1;
  }
  _thingspeeakkey = key;
  return 0;
}

uint8_t DFRobot_WiFi_IoT_Module_UART::thingSpeakSendMessage(char* data1, char *data2, char *data3)
{
  String sendData = "update?api_key=";
  sendData += (String)_thingspeeakkey;
  sendData += "&field1=";
  sendData += (String)data1;
  sendData += "&field2=";
  sendData += (String)data2;
  sendData += "&field3=";
  sendData += (String)data3;
 
  String test = HTTPGet((char*)sendData.c_str());
  if(strcmp((const char*)test.c_str() ,"error") == 0){
    return -1;
  }else{
    return 0;
  }
}

uint8_t DFRobot_WiFi_IoT_Module_UART::receiveData(void)
{
  String data = "";
  uint32_t timecnt = millis();
  while(!_s->available()){
    if((millis()-timecnt) >15000){
      return -1;
    }
  }
  DBG(_s->available());
  while(_s->available() > 0){
    data += char(_s->read());
    uint8_t index = data.lastIndexOf('\r');
    if(index != 255){
      break;
    }
    delay(5);
  }

  DBG(data);
  // Serial.println("------");
  // Serial.println(data);
  uint8_t length = data.length();
  uint8_t index = data.indexOf('\r');
  String subData = data.substring(0,index);
  splitString(_receiveStringIndex,subData,"|");
  return 0;
}

uint8_t DFRobot_WiFi_IoT_Module_UART::splitString(String data[],String str,const char* delimiters)
{
  char *s = (char *)(str.c_str());
  int count = 0;
  data[count] = strtok(s, delimiters);
  while(data[count])
  {
    data[++count] = strtok(NULL, delimiters);
  }
  return count;
}

uint8_t DFRobot_WiFi_IoT_Module::beebotteBegin(char *token)
{
  if(!token){
    return -1;
  }
  _token  = token;
  return 0;
}

uint8_t DFRobot_WiFi_IoT_Module_I2C::beebotteSendMessage(char *channel, char *resource ,char *data)
{
  String recv = "{\"data\":" + (String)data + "}\r\n";
  String postRequest ="v1/data/write/";
  postRequest += (String)channel;
  postRequest += "/";
  postRequest += (String)resource;
  postRequest += "?token=";
  postRequest += (String)_token;
  postRequest += ',';
  postRequest += (String)recv;
  uint8_t recvHTTPData[100];
  manageFunction(IOT_RUN_COMMAND, HTTP_POST_URL_CON, (uint8_t*)postRequest.c_str(),postRequest.length());
  uint32_t startingTime = millis();
  
    uint8_t state = parameterReturn(HTTP_NORMAL_RETURN, HTTP_ERROR_RETURN, &recvHTTPData[0]);
    if( state == 0){
      return 0;
	  DBG("S");
    }else if(state == 2){
      return 2;
    }else{
      return 1;
    }
    
}

uint8_t DFRobot_WiFi_IoT_Module_UART::beebotteSendMessage(char *channel, char *resource ,char *data)
{
  String recv = "{\"data\":" + (String)data + "}\r\n";  
        
  String postRequest ="v1/data/write/";
  postRequest += (String)channel;
  postRequest += "/";
  postRequest += (String)resource;
  postRequest += "?token=";
  postRequest += (String)_token;
  DBG(postRequest);
  String test = HTTPPost((char*)postRequest.c_str(),(char*)recv.c_str());
  if(strcmp((const char*)test.c_str() ,"error") == 0){
    return -1;
  }else{
    return 0;
  }
}
