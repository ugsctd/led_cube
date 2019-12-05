
Code based on ESP8266 Wordclock
by Thoralt Franz, https://github.com/thoralt

The ESPAsyncWebServer adaptation done by my work mate. 

The cube is zirrfa 8x8x8 LED cube: https://www.aliexpress.com/item/32700909987.html

The cube I built is made with vertical layers of diodes (back to front: red, green, blue, red), as opposed to the original horizontal build. This implies the naming convention of animations - magenta, cyan and yellow are combinations of the colors. Yellow is not really yellow tho :/ And don't judge the job I did with the cube, I know it's terrible.. it was my first actual soldering project :P

I put the D1 mini below the cube and stick it with a double sided tape. I put it unfortunetly in front, so the socket is in front of the cube.. The way it's connected, you can power the cube with both the original cable and with a micro USB cable. 

The box is some cheap sheet of plexi/acryl from a local hardware store for 3 euros. Cut with a chisel so it looks akward and uneven. I would recommend putting more thought to it and preferably cut the plexi with some proper tool. You can get the original plexiglass box  from Aliexpress for about 15 euros (that's why I put it together myself :P).

You need 3 cables to connect the cube: 5V, GND and one of the others set to serial Out. Serial in is irrelevant as the cube doesn't really talk back.

## Features:
* uses the async web server implementation for esp8266 which is just amazing
* works over wifi
* animations: 
* rise, fall, say a string, say a char
    all with parameters: how fast, and on which led layer to display
 
* OTA upgrades

## TODOS:
* the cube crashes on blinking animation
* time animation - both binary and somehow digital
* <del>ntp! as I broke it somehow</del>
* offline support
* misconfiguration display - no wifi, needs password, welcome, etc.. i think i got the logos, but they don't work


## Prerequisites 
* Install Arduino IDE (can be zipped version too) from https://www.arduino.cc/en/main/software
* Follow D1 mini board installation procedure from: https://github.com/esp8266/Arduino
* Download following libraries:
    * https://github.com/me-no-dev/ESPAsyncWebServer
    * https://github.com/me-no-dev/ESPAsyncTCP
    * https://github.com/alanswx/ESPAsyncWiFiManager
* Install from Arduino IDE:
    * ArduinoJson
    * WiFiManager from tzapu (i think)
* to run the code properly, you need to push the data to spiffs
