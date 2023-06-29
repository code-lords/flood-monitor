/*
 * ESP8266 wifi module Interfacing with Arduino Uno
 * http://www.electronicwings.com
 */

#include "ESP8266_AT.h"

/* Select Demo */
// #define RECEIVE_DEMO			/* Define RECEIVE demo */
#define SEND_DEMO /* Define SEND demo */

/* Define Required fields shown below */
#define DOMAIN "api.thingspeak.com"
#define PORT "80"
#define API_WRITE_KEY "2ZWG94PR66AK5GG6"
#define CHANNEL_ID "2186309"

#define SSID "A15"
#define PASSWORD "01234567"
#define SAFE_MODE_LED 8
#define WARNING_MODE_LED 9
#define DANGER_MODE_LED 10
int max_level = 310;
int danger_start = 260;
int warning_start = 220;


int sensorPin = A0; // select the input pin for the potentiometer

char _buffer[150];
uint8_t Connect_Status;
#ifdef SEND_DEMO
uint8_t Sample = 0;
#endif

void setup()
{
  Serial.begin(115200);

  while (!ESP8266_Begin())
    ;
  ESP8266_WIFIMode(BOTH_STATION_AND_ACCESPOINT);          /* 3 = Both (AP and STA) */
  ESP8266_ConnectionMode(SINGLE);                         /* 0 = Single; 1 = Multi */
  ESP8266_ApplicationMode(NORMAL);                        /* 0 = Normal Mode; 1 = Transperant Mode */
  if (ESP8266_connected() == ESP8266_NOT_CONNECTED_TO_AP) /*Check WIFI connection*/
    ESP8266_JoinAccessPoint(SSID, PASSWORD);              /*Connect to WIFI*/
  ESP8266_Start(0, DOMAIN, PORT);
  pinMode(SAFE_MODE_LED, OUTPUT);
  pinMode(WARNING_MODE_LED, OUTPUT);
  pinMode(DANGER_MODE_LED, OUTPUT);

  digitalWrite(SAFE_MODE_LED, HIGH);
}

void loop()
{
  digitalWrite(DANGER_MODE_LED, LOW);
  digitalWrite(WARNING_MODE_LED, LOW);
  blink(SAFE_MODE_LED);
  int sensorValue = analogRead(sensorPin);
   if (sensorValue >= danger_start)
  {
    blink(DANGER_MODE_LED);
  }

  else if (sensorValue >= warning_start )
  {
    blink(WARNING_MODE_LED);
  }
  
  Connect_Status = ESP8266_connected();
  if (Connect_Status == ESP8266_NOT_CONNECTED_TO_AP) /*Again check connection to WIFI*/
    ESP8266_JoinAccessPoint(SSID, PASSWORD);         /*Connect to WIFI*/
  if (Connect_Status == ESP8266_TRANSMISSION_DISCONNECTED)
    ESP8266_Start(0, DOMAIN, PORT); /*Connect to TCP port*/

#ifdef SEND_DEMO
  memset(_buffer, 0, 150);
  sprintf(_buffer, "GET /update?api_key=%s&field1=%d", API_WRITE_KEY, sensorValue); /*connect to thingspeak server to post data using your API_WRITE_KEY*/
  ESP8266_Send(_buffer);
  delay(1500); /* Thingspeak server delay */
#endif

#ifdef RECEIVE_DEMO
  memset(_buffer, 0, 150);
  sprintf(_buffer, "GET /channels/%s/feeds/last.txt", CHANNEL_ID); /*Connect to thingspeak server to get data using your channel ID*/
  ESP8266_Send(_buffer);
  Read_Data(_buffer);
  delay(600);
#endif
}
void blink(int led)
{
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
}
