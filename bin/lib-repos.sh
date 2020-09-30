# lib-repos.sh
#
# a list of all the library repos we use, for the build to patch
# for the unPhone IOExpander

# git repos that need patching
https://github.com/matthijskooijman/arduino-lmic.git patch # LMIC/LoRa
https://github.com/adafruit/Adafruit-GFX-Library patch # graphics abstraction
https://github.com/adafruit/Adafruit_HX8357_Library  patch # LCD
https://github.com/adafruit/Adafruit_STMPE610        patch # touch screen
https://github.com/adafruit/Adafruit_VS1053_Library  patch # MP3/synth/ogg
# TODO don't need music any more

# esp32 core package libraries that need patching
../sdks/Arduino/hardware/espressif/esp32/libraries/SD patch # SD cards

# libs that don't need patching
https://github.com/adafruit/Adafruit_LSM303DLHC    # accelerometer
https://github.com/adafruit/Adafruit_Sensor        # high-level sensor abst
https://github.com/sui77/rc-switch                 # 433 MHz switching
https://github.com/marcoschwartz/aREST             # REST lib used by robocar
https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library # motor feather
https://github.com/beegee-tokyo/DHTesp             # temp and humidity sensor
https://github.com/adafruit/Adafruit_TSL2561       # light/IR sensor (square)
https://github.com/adafruit/Adafruit_TSL2591_Library # light/IR sensor (round)
https://github.com/adafruit/Adafruit_NeoPixel      # neopixels
https://github.com/gmag11/WifiLocation             # google's geolocation API
GP2Y1010_DustSensor                                # dust sensor
            # (used to be at: https://github.com/nara256/GP2Y1010_DustSensor)
# https://github.com/nkolban/esp32-snippets.git    # Neil Kolban's library
https://github.com/me-no-dev/ESPAsyncWebServer.git # asynchronous web svr
https://github.com/me-no-dev/AsyncTCP.git          # async tcp used by svr
# infra-red remote controls... but not a usual lib format :(
# https://github.com/SensorsIot/Definitive-Guide-to-IR/tree/master/ESP32-IRremote
https://github.com/tzapu/WiFiManager.git           # wifi connection mgmt
https://github.com/PaulStoffregen/OneWire          # 1 wire comms
https://github.com/bblanchon/ArduinoJson           # manipulate JSON data
https://github.com/milesburton/Arduino-Temperature-Control-Library # DallasTem
https://github.com/openenergymonitor/EmonLib       # open energy monitor

# latest version requires SPIFlash and SdFat, but these don't compile on IDE
# 1.9.0... ho hum
https://github.com/adafruit/Adafruit_ImageReader   # bmp draw to LCD library
#https://github.com/adafruit/Adafruit_SPIFlash      # used by image reader
#https://github.com/adafruit/SdFat                  # ditto

https://github.com/adafruit/Adafruit_DotStar       # dot star LEDs
https://github.com/adafruit/Adafruit_NeoMatrix     # NeoPix matrices

ESPWebServer                                       # simple web server ex 8266
https://github.com/adafruit/Adafruit_IO_Arduino    # adafruit cloud API
https://github.com/adafruit/Adafruit_MQTT_Library  # used by adafruit.io
https://github.com/arduino-libraries/ArduinoHttpClient # ditto
https://github.com/adafruit/Adafruit_SSD1306       # oled featherwing display
https://github.com/JChristensen/movingAvg          # simple running averages
