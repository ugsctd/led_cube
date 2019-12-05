// ESP8266 Wordclock
// Copyright (C) 2016 Thoralt Franz, https://github.com/thoralt
//
//  See config.cpp for description.
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
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <IPAddress.h>

// structure with configuration data to be stored in EEPROM
typedef struct _config_struct
{
	uint32_t magic;
	uint8_t ntpserver[4];

	bool heartbeat;
	int delay;
	uint32_t timeZone;
	uint32_t animationType;
	uint32_t color;
	String text;
} config_struct;

// #define EEPROM_SIZE 512
#define EEPROM_SIZE 4096

class ConfigClass
{
public:
	// public methods
	ConfigClass();
	virtual ~ConfigClass();
	void begin();
	void save();
	void saveDelayed();
	void load();
	void reset();
	void formatEeprom();

	// public configuration variables
	IPAddress ntpserver = IPAddress(0, 0, 0, 0);
	bool heartbeat = false;
	int delay = 100;
	bool debugMode = false;

	uint32_t currentAnimation = 1; //Fall
	uint32_t currentColor = 6;	 //All
	String currentText = "icecream!";

	int updateProgress = 0;
	int timeZone = 1;
	int delayedWriteTimer = 0;
	bool delayedWriteFlag = false;

private:
	// copy of EEPROM content
	config_struct *config = (config_struct *)eeprom_data;
	uint8_t eeprom_data[EEPROM_SIZE];
};

extern ConfigClass Config;

#endif /* CONFIG_H_ */
