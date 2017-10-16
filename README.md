# sonoff_b1
Custom firmware for SONOFF B1, enabling MQTT communication

The code to interface with the LED driver has been borrowed from [Tasmota](https://github.com/arendst/Sonoff-Tasmota)

## Requirements
* Arduino IDE with [support for ESP8266](https://github.com/esp8266/Arduino)
* [Async MQTT client](https://github.com/marvinroger/async-mqtt-client)
* [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)


## Arduino IDE flashing settings

* Type of board: Generic ESP8266
* Flash Mode: DOUT
* Flash Frequency: 40MHz
* CPU Frequency: 80MHz
* Flash Size: 1M (64K SPIFFS)
* Debug Port: Disabled
* Debug Level: None
* Reset Method: ck
* Upload Speed: 115200

Note: Even though the SONOFF B1 is equipped with an ESP8285, using the ESP8285 of the Arduino IDE will prevent OTA updates from working.
