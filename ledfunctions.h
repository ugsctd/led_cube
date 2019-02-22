
// ESP8266 Wordclock
// Copyright (C) 2016 Thoralt Franz, https://github.com/thoralt
//
//  See ledfunctions.cpp for description.
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
#ifndef _LEDFUNCTIONS_H_
#define _LEDFUNCTIONS_H_

#include <NeoPixelBus.h>

#include <stdint.h>
#include <vector>

#include "config.h"

typedef struct _xy_t
{
	int xTarget, yTarget, x, y, delay, speed, counter;
} xy_t;

#define NUM_MATRIX_OBJECTS 25
#define NUM_STARS 10
#define NUM_BRIGHTNESS_CURVES 2

class LEDFunctionsClass
{
public:
	LEDFunctionsClass();
	~LEDFunctionsClass();
	void begin(int pin);
	void process();
	void setTime(int h, int m, int s, int ms);
	void setBrightness(int brightness);
	void setMode(DisplayMode newMode);
	void show();

	static int getOffset(int x, int y);
	static const int width = 11;
	static const int height = 10;
	uint8_t currentValues[NUM_PIXELS * 3];

  void changeDialect (int id);
  void changeToRuhr();
  void changeToWessi();
  void changeToOssi();
  void changeToSchwabe();

private:
  static const palette_entry firePalette[];
  static const palette_entry plasmaPalette[];

	DisplayMode mode = DisplayMode::plain;


  
	uint8_t targetValues[NUM_PIXELS * 3];
	int heartBrightness = 0;
	int heartState = 0;
	int brightness = 96;
	int h = 0;
	int m = 0;
	int s = 0;
	int ms = 0;
	int lastM = -1;
	int lastH = -1;

	void fillBackground(int seconds, int milliseconds, uint8_t *buf);
	void renderRed();
	void renderGreen();
	void renderBlue();
	
	void renderUpdate();
	void renderUpdateComplete();
	void renderUpdateError();
	void renderWifiManager();
	void renderTime(uint8_t *target, int h, int m, int s, int ms);
	void fade();
	void set(const uint8_t *buf, palette_entry palette[]);
	void set(const uint8_t *buf, palette_entry palette[], bool immediately);
	void setBuffer(uint8_t *target, const uint8_t *source, palette_entry palette[]);

	// this mapping table maps the linear memory buffer structure used throughout the
	// project to the physical layout of the LEDs
};

extern LEDFunctionsClass LED;

#endif
