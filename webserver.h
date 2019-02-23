// ESP8266 Wordclock
// Copyright (C) 2016 Thoralt Franz, https://github.com/thoralt
//
//  See webserver.cpp for description.
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
#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#include <stdint.h>
#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>

#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"

#include "config.h"

class WebServerClass
{
public:
	WebServerClass();
	virtual ~WebServerClass();
	void begin();
	void process();

private:
	// JsonObject &buildConfigurationString(JsonBuffer &jsonBuffer);

	AsyncWebServer server = AsyncWebServer(80);
	AsyncWebSocket ws = AsyncWebSocket("/ws"); // access at ws://[esp ip]/ws

	void handleSaveConfig(AsyncWebServerRequest *request);
	void handleLoadConfig(AsyncWebServerRequest *request);
	void handleResetConfig(AsyncWebServerRequest *request);
	void handleGetColors();
	void handleSetColor(AsyncWebServerRequest *request);
	void handleNotFound();
	void handleSetTimeZone(AsyncWebServerRequest *request);
	void handleGetTimeZone();
	void handleSetDialect(AsyncWebServerRequest *request);
	void handleGetDialect();
	void handleSetMode(AsyncWebServerRequest *request);
	void handleGetMode();
	void handleSetHeartbeat(AsyncWebServerRequest *request);
	void handleGetHeartbeat();
	void handleSetEsIst(AsyncWebServerRequest *request);
	void handleGetEsIst();
	void handleInfo(AsyncWebServerRequest *request);
	void handleH(AsyncWebServerRequest *request);
	void handleM(AsyncWebServerRequest *request);
	void handleR(AsyncWebServerRequest *request);
	void handleG(AsyncWebServerRequest *request);
	void handleB(AsyncWebServerRequest *request);
	void handleDebug(AsyncWebServerRequest *request);
	void handleGetBrightness();
	void handleSetBrightness(AsyncWebServerRequest *request);
	void handleSetBrightnessTable(AsyncWebServerRequest *request);
	void handleGetADC(AsyncWebServerRequest *request);
	void handleGetNtpServer();
	void handleSetNtpServer(AsyncWebServerRequest *request);
	void handleGetSettings(AsyncWebServerRequest *request);
	// void extractColor(AsyncWebServerRequest *request, String argName, palette_entry& result);
};

extern WebServerClass WebServer;

#endif
