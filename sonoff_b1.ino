/*
 * CUSTOM SONOFF FIRMWARE
 * Maxime MOREILLON
 * 
 * Board type: Generic ESP8285
 */


// Libraries
#include <ESP8266WiFi.h> // Main ESP8266 library
#include <ArduinoOTA.h> // OTA update library
#include <WiFiUdp.h> // Required for OTA
#include <AsyncMqttClient.h> // MQTT library
#include <Ticker.h> // Used when reconecting MQTT upon wifi drop

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

char* light_brightness = "255";
int light_brightness_int = 255;

char* light_rgb;
int light_r_int = 255;
int light_g_int = 255;
int light_b_int = 255;

void setup()
{
  // Mandatory initial delay
  delay(10);

  // Serial init
  Serial.begin(115200);
  Serial.println(); // Separate serial stream from initial gibberish
  Serial.println(F(__FILE__ " " __DATE__ " " __TIME__));

  pinMode(DI_pin, OUTPUT);
  pinMode(DCK_pin, OUTPUT);
  sl_my92x1_init();
  sl_my92x1_duty(light_r_int,light_g_int,light_b_int,light_brightness_int,light_brightness_int);
  
  wifi_setup();
  MQTT_setup();
  OTA_setup();

  wifi_connect();
}


void loop() {
  ArduinoOTA.handle();
  
}
