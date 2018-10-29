#include "Arduino.h"
#include "AppDebug.h"
#include "ESP8266WiFi.h"
#include "PubSubClient.h"
#include "ArduinoJson.h"
#include "Ticker.h"
#include "EEPROM.h"

WiFiClient espClient;
PubSubClient client(espClient);

#define WIFI_SSID       "HPT CNC1"    //thay doi ten mang wifi 
#define WIFI_PASSWORD   "hpt12345"   //thay doi password cho wifi

#define MQTT_SERVER     "192.168.1.34"
//#define MQTT_SERVER     "test.mosquitto.org"
//#define MQTT_SERVER     "mqtt-dashboard.com"
//#define MQTT_SERVER     "broker.mqtt-dashboard.com"

#define MQTT_PORT   1883

#define MQTT_client "ESP8266Client2"

#define COMMAND_1   "drawer2"
#define COMMAND_2   "rgb"

#define MOTORUP 1
#define MOTORDOWN 2
#define MOTORSTOP 3


#define LED_TEST D8     // D0 on board
#define LED_TEST_MOTOR  D4 // D4 onchip GPIO2
#define PIN_CONFIG D3       // D3 flash GPIO0
#define PIN_PUL_MOTOR D0    //GPIO16 D0
#define PIN_DIR_MOTOR D5    //GPIO14
#define PIN_ENCODER_MOTOR D6 //1 TX    //D6 GPIO12    
#define PIN_LED_RED D2  //GPIO4
#define PIN_LED_GREEN D7    //GPIO13
#define PIN_LED_BLUE D1     //GPIO5
#define sample_time 25      //thoi gian lay mau la 25 ms de tinh van toc
#define SPEED_DEFAUT 120    //van toc dong co khi hoat dong binh thuong
#define ERROR_SPEED 50      //sai so cho phep cua van toc, neu nam ngoai khoang nay thi coi nhu co vat can

#define RESPONSE_LENGTH 512     //do dai data nhan ve tu tablet
#define EEPROM_WIFI_SSID_START 0
#define EEPROM_WIFI_SSID_END 32
#define EEPROM_WIFI_MAX_CLEAR 512
#define EEPROM_WIFI_PASS_START 33
#define EEPROM_WIFI_PASS_END 64
#define EEPROM_WIFI_IP_START 65
#define EEPROM_WIFI_IP_END 95
#define EEPROM_WIFI_LED_RED_START 100
#define EEPROM_WIFI_LED_RED_END 105
#define EEPROM_WIFI_LED_GREEN_START 106
#define EEPROM_WIFI_LED_GREEN_END 110
#define EEPROM_WIFI_LED_BLUE_START 111
#define EEPROM_WIFI_LED_BLUE_END 115


//all of function
void setupLedbegin();
void SetupNetwork();
void goUpMotor();
void goDownMotor();
void stopMotor();
void ControlLed();
void setPulMotor();

bool flagControlLed = false;
uint8_t flagModeMotor = 0;
uint8_t red_before, green_before, blue_before;
uint8_t red_after, green_after, blue_after;
String datareceive;
// Ticker tickerSetApMode(setLedApMode, 200, 0);   //every 200ms
// Ticker tickerSpeed(caculate_Speed, 25, 0);  //every 25ms
Ticker tickerSetMotor(setPulMotor, 500, 0, MICROS_MICROS);  //every 0.5ms
