#include <Ticker.h>
// #include <stdint.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <Ticker.h>
// #include <WiFi.h>
#include <ESPAsyncWiFiManager.h>
#include <WiFiUdp.h>

#include "webserver.h"
#include "config.h"
#include "osapi.h"
#include "cube.h"
#include "ntp.h"

// #define PRODUCTION
#ifndef PRODUCTION

// #include "RemoteDebug.h"
// RemoteDebug Debug;

// bool debug = false;

// #define VERBOSE(...)                 \
//   if (Debug.isActive(Debug.VERBOSE)) \
//   {                                  \
//     Debug.printf(__VA_ARGS__);       \
//   }
// #define DEBUG(...)                 \
//   if (Debug.isActive(Debug.DEBUG)) \
//   {                                \
//     Debug.printf(__VA_ARGS__);     \
//   }
// #define INFO(...)                 \
//   if (Debug.isActive(Debug.INFO)) \
//   {                               \
//     Debug.printf(__VA_ARGS__);    \
//   }
// #define WARNING(...)                 \
//   if (Debug.isActive(Debug.WARNING)) \
//   {                                  \
//     Debug.printf(__VA_ARGS__);       \
//   }
// #define ERROR(...)                 \
//   if (Debug.isActive(Debug.ERROR)) \
//   {                                \
//     Debug.printf(__VA_ARGS__);     \
//   }

#endif

// #define LED_BUILTIN 2

extern bool shouldReboot;

// Network related variables
int OTA_in_progress = 0;

// Timer related variables
#define TIMER_RESOLUTION 10
Ticker timer;
int h = 0;
int m = 0;
int s = 0;
int ms = 0;
int lastSecond = -1;
bool timeVarLock = false;
bool startup = true;

int updateCountdown = 25;

// Increments time, decrements timeout and NTP timer
void timerCallback()
{
  // update time variables
  if (!timeVarLock)
  {
    timeVarLock = true;
    ms += TIMER_RESOLUTION;
    if (ms >= 1000)
    {
      ms -= 1000;
      if (++s > 59)
      {
        s = 0;
        if (++m > 59)
        {
          m = 0;
          if (++h > 23)
            h = 0;
        }
      }
      timeVarLock = false;
    }

    // decrement delayed EEPROM config timer
    if (Config.delayedWriteTimer)
    {
      Config.delayedWriteTimer--;
      if (Config.delayedWriteTimer == 0)
        Config.delayedWriteFlag = true;
    }

    // blink onboard LED if heartbeat is enabled
    // if (ms == 0 && Config.heartbeat) digitalWrite(LED_BUILTIN, LOW);
    // else digitalWrite(LED_BUILTIN, HIGH);
  }
}
// Couldn't find saved network or no network saved
void configModeCallback(AsyncWiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
  // Config.delay = 250;
  // Cube.ChangeAnimation(AnimationType::Say, 0, "  @@@  WiFi", ColumnColor::Yellow);
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

// Is called by the NTP class upon successful reception of an NTP data packet. Updates
// the global hour, minute, second and millisecond values.
void NtpCallback(uint8_t _h, uint8_t _m, uint8_t _s, uint8_t _ms)
{
  Serial.printf("Callback local time: %02i:%02i:%02i.%02i\r\n", _h, _m, _s, _ms);

  // this worked without the lock..
  h = _h;
  m = _m;
  s = _s;
  ms = _ms;

  Serial.printf("Callback stored time: %02i:%02i:%02i.%02i\r\n", h, m, s, ms);
}

// Initializes everything
void setup()
{
  // serial port
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println(F("ESP8266 cube_async setup() begin"));

  // timer
  Serial.println(F("Starting timer"));
  timer.attach(TIMER_RESOLUTION / 1000.0, timerCallback);

  // configuration
  Serial.println(F("Loading configuration"));
  Config.begin();

  // LEDs
  Serial.println(F("Taking over the LED cube"));
  Cube.setup(true);
  // WiFi
  Serial.println(F("Initializing WiFi"));

  AsyncWebServer server(80);
  DNSServer dns;

  AsyncWiFiManager wifiManager(&server, &dns);
  wifiManager.setAPCallback(configModeCallback);
  if (!wifiManager.autoConnect("cube"))
  {
    Serial.println(F("failed to connect, timeout"));
    delay(1000);
    ESP.reset();
  }

  // Setup debugging capabilites
  // #ifndef PRODUCTION
  //   Debug.begin("cube");
  //   Debug.setResetCmdEnabled(true); // Enable the reset command
  //   Debug.showTime(true);           // To show time
  //   Debug.showColors(true);         // Colors
  //   Debug.setSerialEnabled(true);   // if you wants serial echo - only recommended if ESP8266 is plugged in USB
  // #endif

  // OTA update
  Serial.println(F("Initializing OTA"));
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname("cube");
  //ArduinoOTA.setPassword((const char *)"123");
  ArduinoOTA.onStart([]() {
    // Config.delay = 250;
    // Cube.ChangeAnimation(AnimationType::Say, 0, "?", ColumnColor::Cyan);
    Config.updateProgress = 0;
    OTA_in_progress = 1;
    Serial.println(F("OTA Start"));
  });
  ArduinoOTA.onEnd([]() {
    // Cube.ChangeAnimation(AnimationType::Say, 0, "Done!!!!", ColumnColor::Green);

    Serial.println(F("\nOTA End"));
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Config.updateProgress = progress * 110 / total;
    // ColumnColor color = Config.updateProgress > 80 ? ColumnColor::Cyan : Config.updateProgress > 60 ? ColumnColor::Green : Config.updateProgress > 40 ? ColumnColor::Yellow : ColumnColor::Red;

    // Cube.ChangeAnimation(AnimationType::Say, 0, String(Config.updateProgress), color);

    Serial.printf("OTA Progress: %u%%\r\n", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    // Cube.ChangeAnimation(AnimationType::Say, 0, " OTA Error!!  ", ColumnColor::RedFront);
    Serial.printf("OTA Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  // NTP
  Serial.println(F("Starting NTP module"));
  NTP.begin(Config.ntpserver, NtpCallback, Config.timeZone, true);

  // web server
  Serial.println(F("Starting HTTP server"));
  WebServer.begin();

  startup = false;
  Serial.println(F("Startup complete."));
}

void loop()
{
  if (shouldReboot)
  {
    Serial.println(F("Rebooting..."));
    delay(100);
    ESP.restart();
  }

  // do OTA update stuff
  ArduinoOTA.handle();

  // update LEDs
  Cube.printFrame();

  // do not continue if OTA update is in progress
  // OTA callbacks drive the LED display mode and OTA progress
  // in the background, the above call to LED.process() ensures
  // the OTA status is output to the LEDs
  if (OTA_in_progress)
    return;

  // save configuration to EEPROM if necessary
  if (Config.delayedWriteFlag)
  {
    Serial.println(F("Config timer expired, writing configuration.\r\n"));
    Config.delayedWriteFlag = false;
    Config.save();
  }

  // #ifndef PRODUCTION
  //   Debug.handle();
  // #endif
}
// ./esptool.py --port /dev/tty.usbserial --baud 460800 write_flash --flash_size=8m 0 /var/folders/yh/bv744591099f3x24xbkc22zw0000gn/T/build006b1a55228a1b90dda210fcddb62452.tmp/test.ino.bin
// FlashSize 1M (128k SPIFFS)
// C:\Python27\python.exe "C:\Program Files\esptool\espota.py" --ip=192.168.178.95 --port=8266 --progress --file=${workspace_loc}\${project_path}\Release\${project_name}.bin
