// ESP8266 Wordclock
// Copyright (C) 2016 Thoralt Franz, https://github.com/thoralt
//
//  This module encapsulates a small webserver. It replies to requests on port 80
//  and triggers actions, manipulates configuration attributes or serves files
//  from the internal flash file system.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include "webserver.h"
#include "ntp.h"
#include "cube.h"

bool shouldReboot = false;
JsonObject &buildConfigurationString(JsonBuffer &jsonBuffer)
{

  JsonObject &json = jsonBuffer.createObject();

  json["ntp"] = Config.ntpserver.toString();

  int mode = 0;
  switch (Config.defaultMode)
  {
  case DisplayMode::plain:
    mode = 0;
    break;
  case DisplayMode::fade:
    mode = 1;
    break;
  case DisplayMode::flyingLettersVerticalUp:
    mode = 2;
    break;
  case DisplayMode::flyingLettersVerticalDown:
    mode = 3;
    break;
  default:
    mode = 0;
    break;
  }

  json["mode"] = String(mode);

  if (Config.heartbeat)
    json["heartbeat"] = "1";
  else
    json["heartbeat"] = "0";

  json["dialect"] = String(Config.dialect);
  json["timezone"] = String(Config.timeZone);

  return json;
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void handleConfigMessage(String msg)
{

  String key = getValue(msg, ':', 0);
  String value = getValue(msg, ':', 1);

  // if (key == "heartbeat" && value != ""){
  //   Config.heartbeat = (value == "1");
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    //client->printf("Configuration Broadcast for UID: %u", client->id());
    //client->ping();

    StaticJsonBuffer<512> jsonBuffer;
    JsonObject &json = buildConfigurationString(jsonBuffer);
    size_t len = json.measureLength();
    AsyncWebSocketMessageBuffer *buffer = server->makeBuffer(len); //  creates a buffer (len + 1) for you.
    if (buffer)
    {
      json.printTo((char *)buffer->get(), len + 1);
      client->text(buffer);
    }
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
  }
  else if (type == WS_EVT_ERROR)
  {
    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t *)arg), (char *)data);
  }
  else if (type == WS_EVT_PONG)
  {
    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char *)data : "");
  }
  else if (type == WS_EVT_DATA)
  {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    String msg = "";
    if (info->final && info->index == 0 && info->len == len)
    {
      //the whole message is in a single frame and we got all of it's data
      Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);

      if (info->opcode == WS_TEXT)
      {
        for (size_t i = 0; i < info->len; i++)
        {
          msg += (char)data[i];
        }
      }
      else
      {
        char buff[3];
        for (size_t i = 0; i < info->len; i++)
        {
          sprintf(buff, "%02x ", (uint8_t)data[i]);
          msg += buff;
        }
      }
      Serial.printf("%s\n", msg.c_str());

      if (info->opcode == WS_TEXT)
        handleConfigMessage(msg);
      else
        client->binary("I got your binary message");
    }
    else
    {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if (info->index == 0)
      {
        if (info->num == 0)
          Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
        Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);

      if (info->opcode == WS_TEXT)
      {
        for (size_t i = 0; i < info->len; i++)
        {
          msg += (char)data[i];
        }
      }
      else
      {
        char buff[3];
        for (size_t i = 0; i < info->len; i++)
        {
          sprintf(buff, "%02x ", (uint8_t)data[i]);
          msg += buff;
        }
      }
      Serial.printf("%s\n", msg.c_str());

      if ((info->index + len) == info->len)
      {
        Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if (info->final)
        {
          Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
          if (info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}

// global instance
WebServerClass WebServer = WebServerClass();

// Sets up internal handlers and starts the server at port 80
void WebServerClass::begin()
{
  Serial.println("Starting webserver");
  SPIFFS.begin();

  // this->server = new ESP8266WebServer(80);

  AsyncEventSource events("/events"); // event source (Server-Sent events)

  // server.on("/index", HTTP_ANY, [](AsyncWebServerRequest *request) {
  //   request->send(SPIFFS, "/index.htm");
  // });

  // server.on("/jscolor", HTTP_ANY, [](AsyncWebServerRequest *request) {
  //   request->send(SPIFFS, "/jscolor.js");
  // });

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html").setCacheControl("max-age:600");

  // attach AsyncWebSocket
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.on("/info", HTTP_ANY, std::bind(&WebServerClass::handleInfo, this, std::placeholders::_1));
  server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>");
  });
  server.on("/getsettings", HTTP_ANY, std::bind(&WebServerClass::handleGetSettings, this, std::placeholders::_1));
  server.on("/getadc", HTTP_ANY, std::bind(&WebServerClass::handleGetADC, this, std::placeholders::_1));

  server.on("/saveconfig", HTTP_ANY, std::bind(&WebServerClass::handleSaveConfig, this, std::placeholders::_1));
  server.on("/loadconfig", HTTP_ANY, std::bind(&WebServerClass::handleLoadConfig, this, std::placeholders::_1));
  server.on("/resetconfig", HTTP_ANY, std::bind(&WebServerClass::handleResetConfig, this, std::placeholders::_1));

  server.on("/debug", HTTP_ANY, std::bind(&WebServerClass::handleDebug, this, std::placeholders::_1));

  server.on("/setheartbeat", HTTP_ANY, std::bind(&WebServerClass::handleSetHeartbeat, this, std::placeholders::_1));
  server.on("/setcolor", HTTP_ANY, std::bind(&WebServerClass::handleSetColor, this, std::placeholders::_1));
  server.on("/setntpserver", HTTP_ANY, std::bind(&WebServerClass::handleSetNtpServer, this, std::placeholders::_1));
  server.on("/letter/", HTTP_ANY, std::bind(&WebServerClass::handleLetter, this, std::placeholders::_1));
  server.on("/off", HTTP_ANY, std::bind(&WebServerClass::handleOff, this, std::placeholders::_1));
  server.on("/say", HTTP_ANY, std::bind(&WebServerClass::handleSay, this, std::placeholders::_1));
  server.on("/setDelay", HTTP_ANY, std::bind(&WebServerClass::handleSetDelay, this, std::placeholders::_1));
  server.on("/setmode", HTTP_ANY, std::bind(&WebServerClass::handleSetMode, this, std::placeholders::_1));
  server.on("/settimezone", HTTP_ANY, std::bind(&WebServerClass::handleSetTimeZone, this, std::placeholders::_1));
  server.on("/setdialect", HTTP_ANY, std::bind(&WebServerClass::handleSetDialect, this, std::placeholders::_1));

  // Simple Firmware Update Form
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>");
  });
  server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
    shouldReboot = !Update.hasError();
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", shouldReboot?"OK":"FAIL");
    response->addHeader("Connection", "close");
    request->send(response); }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    if(!index){
      Serial.printf("Update Start: %s\n", filename.c_str());
      Update.runAsync(true);
      if(!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)){
        Update.printError(Serial);
      }
    }
    if(!Update.hasError()){
      if(Update.write(data, len) != len){
        Update.printError(Serial);
      }
    }
    if(final){
      if(Update.end(true)){
        Serial.printf("Update Success: %uB\n", index+len);
      } else {
        Update.printError(Serial);
      }
    } });

  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404);
  });

  server.begin();
  Serial.println("Started Server");
}

//---------------------------------------------------------------------------------------
// handleM
//
// Handles the /m request, increments the minutes counter (for testing purposes)
//
// -> --
// <- --
//---------------------------------------------------------------------------------------
extern int h, m;
void WebServerClass::handleM(AsyncWebServerRequest *request)
{

  request->send(200, "text/plain", "OK");
}

void WebServerClass::handleLetter(AsyncWebServerRequest *request)
{
  //RESTful
  auto url = request->url();
  Serial.println(url);
  auto letter_pos = url.indexOf("letter/") + 7;
  auto letter = url[letter_pos];

  // int headers = request->headers();
  // int i;
  auto header = request->getHeader("value");
  // auto letter = header->value().c_str()[0];

  // Cube.printLetter(letter);

  Serial.println(header->value().c_str());
  // for (i = 0; i < headers; i++)
  // {
  //   auto h = request->getHeader(i);
  //   Serial.printf("HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());

  request->send(200, "text/plain", "OK");
  //   // this->ws.textAll("{\"letter\":\"" + letter + "\"}");
  // }
}
void WebServerClass::handleSay(AsyncWebServerRequest *request)
{
  auto header = request->getHeader("value");

  auto what = header->value();
  // for (int i = 0; i < what.length(); i++)
  // Cube.printLetter(what[i]);

  request->send(200, "text/plain", "OK");
  this->ws.textAll("{\"letter\":\"" + what + "\"}");
}

void WebServerClass::handleOff(AsyncWebServerRequest *request)
{
  // Cube.DemoALL_OFF();
  request->send(200, "text/plain", "OK");
}

void WebServerClass::handleSetDelay(AsyncWebServerRequest *request)
{
  if (request->hasArg("value"))
  {
    int delay = request->arg("value").toInt();
    Config.delay = delay;
    Config.save();
    Serial.print("Set delay to:");
    Serial.println(Config.delay);

    request->send(200, "text/plain", "OK");
    this->ws.textAll("{\"delay\":\"" + String(delay) + "\"}");
  }
}
void WebServerClass::handleDebug(AsyncWebServerRequest *request)
{

  if (request->hasArg("clear"))
  {
  }

  if (request->hasArg("end"))
  {
    Config.debugMode = 0;
  }
  request->send(200, "text/plain", "OK");
}

void WebServerClass::handleGetADC(AsyncWebServerRequest *request)
{
  // int __attribute__ ((unused)) temp = Brightness.value(); // to trigger A/D conversion
  // request->send(200, "text/plain", String(Brightness.avg));
}

//---------------------------------------------------------------------------------------
// handleSetTimeZone
//
//
//
// -> --
// <- --
//---------------------------------------------------------------------------------------
void WebServerClass::handleSetTimeZone(AsyncWebServerRequest *request)
{
  if (request->hasArg("value"))
  {
    int newTimeZone = request->arg("value").toInt();
    if (newTimeZone < -12)
      newTimeZone = -12;
    if (newTimeZone > 14)
      newTimeZone = 14;
    Config.timeZone = newTimeZone;
    Config.save();
    NTP.setTimeZone(Config.timeZone);
  }
  request->send(200, "text/plain", "OK");
  this->ws.textAll("{\"timezone\":\"" + request->arg("value") + "\"}");
}

// handleSetDialect
void WebServerClass::handleSetDialect(AsyncWebServerRequest *request)
{
  if (request->hasArg("value"))
  {
    int newDialect = request->arg("value").toInt();
    Config.dialect = newDialect;
    Config.save();
    // LED.changeDialect(Config.dialect);
  }
  request->send(200, "text/plain", "OK");
  this->ws.textAll("{\"dialect\":\"" + request->arg("value") + "\"}");
}

// Handles the /setmode request. Sets the display mode to one of the allowed values,
// saves it as the new default mode.
void WebServerClass::handleSetMode(AsyncWebServerRequest *request)
{
  AnimationType mode = AnimationType::Invalid;

  if (request->hasArg("value"))
  {
    // handle each allowed value for safety
    if (request->arg("value") == "0")
      mode = AnimationType::Rise;
    if (request->arg("value") == "1")
      mode = AnimationType::Fall;
    if (request->arg("value") == "2")
      mode = AnimationType::Letter;
    if (request->arg("value") == "3")
      mode = AnimationType::Say;
    if (request->arg("value") == "4")
      mode = AnimationType::Wall;
    if (request->arg("value") == "5")
      mode = AnimationType::Blink;
    if (request->arg("value") == "6")
      mode = AnimationType::Time;
    if (request->arg("value") == "7")
      mode = AnimationType::Random;
  }
  ColumnColor color = ColumnColor::Red;
  
  if (request->hasArg("color"))
  {
    // handle each allowed value for safety
    if (request->arg("color") == "0")
      color = ColumnColor::Red;
    if (request->arg("color") == "1")
      color = ColumnColor::Green;
    if (request->arg("color") == "2")
      color = ColumnColor::Blue;
    if (request->arg("color") == "3")
      color = ColumnColor::Cyan;
    if (request->arg("color") == "4")
      color = ColumnColor::Magenta;
    if (request->arg("color") == "5")
      color = ColumnColor::Yellow;
    if (request->arg("color") == "6")
      color = ColumnColor::All;
    if (request->arg("color") == "7")
      color = ColumnColor::RedFront;
    if (request->arg("color") == "8")
      color = ColumnColor::None;
  }
  if (mode == AnimationType::Invalid)
  {
    request->send(400, "text/plain", "ERR");
  }
  else
  {
    Cube.ChangeAnimation(mode, request->arg("param1").toInt(), request->arg("param2"), color);
    request->send(200, "text/plain", "OK");
  }
  this->ws.textAll("{\"mode\":\"" + request->arg("value") + "\"}");
}
////---------------------------------------------------------------------------------------
//// handleGetMode
////
//// Handles the /getmode request and returns the current default display mode.
////
//// -> --
//// <- --
////---------------------------------------------------------------------------------------
//void WebServerClass::handleGetMode(AsyncWebServerRequest *request)
//{
//	int mode = 0;
//	switch(Config.defaultMode)
//	{
//	case DisplayMode::plain:
//		mode = 0; break;
//	case DisplayMode::fade:
//		mode = 1; break;
//	case DisplayMode::flyingLettersVerticalUp:
//		mode = 2; break;
//	case DisplayMode::flyingLettersVerticalDown:
//		mode = 3; break;
//	default:
//		mode = 0; break;
//	}
//	request->send(200, "text/plain", String(mode));
//}
//
////---------------------------------------------------------------------------------------
//// handleNotFound
////
//// Handles all requests not bound to other handlers, tries to serve a file if found in
//// flash, responds with 404 otherwise
////
//// -> --
//// <- --
////---------------------------------------------------------------------------------------
//void WebServerClass::handleNotFound()
//{
//	// first, try to serve the requested file from flash
//	if (!serveFile(this->server->uri()))
//	{
//		// create 404 message if no file was found for this URI
//		String message = "File Not Found\n\n";
//		message += "URI: ";
//		message += this->server->uri();
//		message += "\nMethod: ";
//		message += (this->server->method() == HTTP_GET) ? "GET" : "POST";
//		message += "\nArguments: ";
//		message += request->hasArgs();
//		message += "\n";
//		for (uint8_t i = 0; i < request->hasArgs(); i++)
//		{
//			message += " " + request->hasArgName(i) + ": "
//					+ request->hasArg(i) + "\n";
//		}
//		request->send(404, "text/plain", message);
//	}
//}
//

//// Delivers the currently configured NTP server IP address
//void WebServerClass::handleGetNtpServer()
//{
//	request->send(200, "application/json", Config.ntpserver.toString());
//}
//

// Sets a new IP address for the NTP client
void WebServerClass::handleSetNtpServer(AsyncWebServerRequest *request)
{
  if (request->hasArg("ip"))
  {
    IPAddress ip;
    if (ip.fromString(request->arg("ip")))
    {
      // set IP address in config
      Config.ntpserver = ip;
      Config.save();

      // set IP address in client
      NTP.setServer(ip);
    }
  }
  request->send(200, "application/json", "OK");
  this->ws.textAll("{\"ntp\":\"" + request->arg("ip") + "\"}");
}

// Handles requests to "/info", replies with JSON structure containing system status
void WebServerClass::handleInfo(AsyncWebServerRequest *request)
{
  Serial.println("Info timer");
  StaticJsonBuffer<512> jsonBuffer;
  char buf[512];
  JsonObject &json = jsonBuffer.createObject();
  json["heap"] = ESP.getFreeHeap();
  json["sketchsize"] = ESP.getSketchSize();
  json["sketchspace"] = ESP.getFreeSketchSpace();
  json["cpufrequency"] = ESP.getCpuFreqMHz();
  json["chipid"] = ESP.getChipId();
  json["sdkversion"] = ESP.getSdkVersion();
  json["bootversion"] = ESP.getBootVersion();
  json["bootmode"] = ESP.getBootMode();
  json["flashid"] = ESP.getFlashChipId();
  json["flashspeed"] = ESP.getFlashChipSpeed();
  json["flashsize"] = ESP.getFlashChipRealSize();
  json["resetreason"] = ESP.getResetReason();
  json["resetinfo"] = ESP.getResetInfo();
  

  if (request->hasArg("pretty"))
  {
    json.prettyPrintTo(buf, sizeof(buf));
  }
  else
  {
    json.printTo(buf, sizeof(buf));
  }
  request->send(200, "application/json", buf);
}

// Handles the "/setcolor" request, expects arguments:
//	/setcolor?fg=xxxxxx&bg=yyyyyy&s=zzzzzz
//	with xxxxxx, yyyyyy and zzzzzz being hexadecimal HTML colors (without leading '#')
void WebServerClass::handleSetColor(AsyncWebServerRequest *request)
{
  // this->extractColor(request, "fg", Config.fg);
  // this->extractColor(request, "bg", Config.bg);
  // this->extractColor(request, "s", Config.s);
  request->send(200, "text/plain", "OK");

  // Config.saveDelayed();

  // String message = String("{\"colors\":\"") + String(Config.bg.r) + "," + String(Config.bg.g) + "," + String(Config.bg.b) + "," + String(Config.fg.r) + "," + String(Config.fg.g) + "," + String(Config.fg.b) + "," + String(Config.s.r) + "," + String(Config.s.g) + "," + String(Config.s.b) + String("\"}");

  // this->ws.textAll(message);
}

// Dumps all Config settings as a JSON object
void WebServerClass::handleGetSettings(AsyncWebServerRequest *request)
{
  StaticJsonBuffer<512> jsonBuffer;
  char buf[512];

  JsonObject &json = jsonBuffer.createObject();

  // json["ntp"] = Config.ntpserver.toString();

  int mode = static_cast<int>(Cube.type);

  json["mode"] = String(mode);
  json["delay"] = String(Config.delay);

  // if (Config.heartbeat)
  //   json["heartbeat"] = "1";
  // else
  //   json["heartbeat"] = "0";

  // json["dialect"] = String(Config.dialect);
  // json["timezone"] = String(Config.timeZone);
  // json["test"] = "pass";

  if (request->hasArg("pretty"))
  {
    json.prettyPrintTo(buf, sizeof(buf));
  }
  else
  {
    json.printTo(buf, sizeof(buf));
  }

  request->send(200, "application/json", buf);
}

// Saves the current configuration to EEPROM
void WebServerClass::handleSaveConfig(AsyncWebServerRequest *request)
{
  Config.save();
  request->send(200, "text/plain", "OK");
}

// Loads the current configuration from EEPROM
void WebServerClass::handleLoadConfig(AsyncWebServerRequest *request)
{
  Config.load();
  request->send(200, "text/plain", "OK");
}

// Resets the current configuration to default
void WebServerClass::handleResetConfig(AsyncWebServerRequest *request)
{
  Config.reset();
  request->send(200, "text/plain", "OK");
}

// Sets or resets the heartbeat flag in the configuration based on argument "state"
void WebServerClass::handleSetHeartbeat(AsyncWebServerRequest *request)
{
  Config.heartbeat = (request->hasArg("value") && request->arg("value") == "1");
  Config.save();
  request->send(200, "text/plain", "OK");
  this->ws.textAll("{\"heartbeat\":\"" + request->arg("value") + "\"}");
}

//// Returns the state of the heartbeat flag.
//void WebServerClass::handleGetHeartbeat()
//{
//	if(Config.heartbeat) request->send(200, "text/plain", "1");
//	else request->send(200, "text/plain", "0");
//}
//

//// Outputs the currently active colors as comma separated list for background, foreground
//// and seconds color with 3 values each (red, green, blue)
//void WebServerClass::handleGetColors()
//{
//	String message = String(Config.bg.r) + "," + String(Config.bg.g) + ","
//			+ String(Config.bg.b) + "," + String(Config.fg.r) + ","
//			+ String(Config.fg.g) + "," + String(Config.fg.b) + ","
//			+ String(Config.s.r) + "," + String(Config.s.g) + ","
//			+ String(Config.s.b);
//	request->send(200, "text/plain", message);
//}
