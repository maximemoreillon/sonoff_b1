/*
 * CUSTOM SONOFF B1 FIRMWARE
 * Maxime MOREILLON
 * 
 * Flash settings:
 * Board type: Generic ESP8266 (even though the chip is an ESP8285)
 * Flash mode: DIO (DOUT for newer SONOFF)
 * Flash size: 1M (64 SPIFFS)
 */


// Libraries
#include <ESP8266WiFi.h> // Main ESP8266 library
#include <ArduinoOTA.h> // OTA update library
#include <WiFiUdp.h> // Required for OTA
#include <AsyncMqttClient.h> // MQTT library
#include <Ticker.h> // Used when reconecting MQTT upon wifi drop
#include <ArduinoJson.h>

#include "credentials.h";
#include "kitchen_light_config.h";

// Wifi
Ticker wifi_reconnect_timer;
WiFiEventHandler wifi_connect_handler;
WiFiEventHandler wifi_disconnect_handler;

// MQTT
AsyncMqttClient MQTT_client;
Ticker MQTT_reconnect_timer;
#define MQTT_BROKER_ADDRESS IPAddress(192, 168, 1, 2)
#define MQTT_PORT 1883
#define MQTT_LAST_WILL "OFF"
#define MQTT_QOS 1
#define MQTT_RETAIN true

// PIn mapping
#define DI_pin 12
#define DCK_pin 14

// Other variables
char* light_state = "ON";
int light_brightness = 255;
int light_r = 255;
int light_g = 255;
int light_b = 255;

void setup()
{
  // Mandatory initial delay
  delay(10);

  // Serial init
  Serial.begin(115200);
  Serial.println(); // Separate serial stream from initial gibberish
  Serial.println(F(__FILE__ " " __DATE__ " " __TIME__));

  LED_setup();
  LED_set(light_r,light_g,light_b,light_brightness,light_brightness);
  
  wifi_setup();
  MQTT_setup();
  OTA_setup();

  wifi_connect();
}


void loop() {
  ArduinoOTA.handle();
}
