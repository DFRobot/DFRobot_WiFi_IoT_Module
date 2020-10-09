# DFRobot_WiFi_IoT_Module

This library enables the Wifi IoT module to access IoT platforms like IFTTT, BeeBotte and Easy_IoT, SIot, OneNET, and ThingSpeak.

   
## Table of Contents

* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

 
## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
  /**
   * @brief Initial I2C
   */
  uint8_t begin(void);
  /**
   * @brief Configure WiFi link network
   * @param ssid WiFi name
   * @param pwd Wifi password
   * @return 0: WiFi connected, 1：WiFi connection error, 2: Connecting to WiFi
   */
  uint8_t connectWifi(char *ssid, char *pwd);
  /**
   * @brief MQTT configuration
   * @param server Server address
   * @param port IOT port number
   * @param id   IOT ID name
   * @param pwd  IOT access password
   * @return MQTT connection status
   */ 
  uint8_t MQTTBegin(char *server = NULL, char *port = NULL, char *productID  = NULL, char *pwd = NULL, char* deviceID = NULL);
  /**
   * @brief  HTTP configuration                  
   * @param ip  Server address
   * @return True：succeeded, false: failed
   */
  bool HTTPBegin(uint8_t *ip = NULL);
  /**
   * @brief Title Subscription, up to 5
   * @param topic Subscription channel 
   * @return 0 subscription succeeded, 1 subscription failed, 2 Reached subscription upper-limit, 3 subscription started, null None, 5 no subscription object
   */
  uint8_t subscribe(char *topic = NULL);
  /**
   * @brief Send message to title
   * @param data Data to be sent
   * @param topic    Subscription channel 
   * @return 0：succeeded, -1: failed
   */
  uint8_t publish(char *topic, String data);
  /**
   * @brief  Send any message to the title
   * @param data Data to be sent  
   * @param topic  Subscription channel 
   * @param len Data length
   * @return 0：succeeded, -1: failed
   */
  uint8_t publish(char *topic, uint8_t *data, uint16_t len);
  /**
   * @brief HTTPget URL
   */
  char* HTTPGet(char *getUrl = NULL);
  /**
   * @brief HTTPPost
   * @param postUrl URL
   * @param data   Data to be sent
   */
  char* HTTPPost(char* postUrl, char* data);
  /**
   * @brief  Get version number
   * @return  Version information
   */
  char* getVersion();
  /**
   * @brief    Loop read status
   */
  void loop();
  /**
   * @brief  Set callback function
   */
  void setCallBack(callback call);
  /** 
   * @brief   IFTTT configuration
   * @param event  IFTTT registered event name 
   * @param key    IFTTT registered event key
   * @return  True: configuration succeeded, false: configuration failed
   */
  bool IFTTTBegin(char *event = NULL, char *key = NULL);
  /**
   * @brief  IFTTT send Messages 
   * @param data1 Data to be sent 
   */
  void IFTTTSendMessage(char *data1, char *data2, char *data3);
  /**
   * @brief   Thingspeak configuration
   * @param key Thingspeak Access key
   * @return   True: configuration succeeded, false: configuration failed
   */
  bool thingSpeakBegin(char *key);
  /**
   * @brief  Thingspeak send messages
   * @param data   Data to be sent 
   */
  void thingSpeakSendMessage(char* data1 = NULL, char *data2 = NULL, char *data3 = NULL);
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino uno        |      √       |              |             | 
Mega2560           |      √       |              |             | 
Leonardo           |      √       |              |             | 


## History

- Data 2020-6-17
- Version V1.0


## Credits

Written by Tang(jie.tang@dfrobot.com), 2020. (Welcome to our [website](https://www.dfrobot.com/))





