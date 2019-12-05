#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"

//---------------------------------------------------------------------------------------
// global instance
//---------------------------------------------------------------------------------------
ConfigClass Config = ConfigClass();

// Constructor, loads default values
ConfigClass::ConfigClass()
{
	this->reset();
}

// destructor
ConfigClass::~ConfigClass()
{
}

// Initializes the class and loads current configuration from EEPROM into class members.
void ConfigClass::begin()
{
	EEPROM.begin(EEPROM_SIZE);
	this->load();
}

// Copies the current class member values to EEPROM buffer and writes it to the EEPROM
void ConfigClass::saveDelayed()
{
	this->delayedWriteTimer = 1000; // 10 seconds using 10 ms timer
}

// Copies the current class member values to EEPROM buffer and writes it to the EEPROM.
void ConfigClass::save()
{
	this->delayedWriteFlag = false;

	this->config->timeZone = this->timeZone;
	this->config->delay = this->delay;
	this->config->heartbeat = this->heartbeat;

	this->config->animationType = (uint32_t)this->currentAnimation;
	this->config->color = (uint32_t)this->currentColor;
	this->config->text = this->currentText;
	for (int i = 0; i < 4; i++)
		this->config->ntpserver[i] = this->ntpserver[i];

	for (int i = 0; i < EEPROM_SIZE; i++)
		EEPROM.write(i, this->eeprom_data[i]);
	EEPROM.commit();
}

// Sets default values in EEPROM buffer and member variables.
void ConfigClass::reset()
{
	this->config->magic = 0xDEADBEEF;

	this->config->delay = 40;
	this->delay = this->config->delay;

	this->config->heartbeat = true;
	this->heartbeat = this->config->heartbeat;

	this->config->animationType = (uint32_t)this->currentAnimation;
	this->config->color = (uint32_t)this->currentColor;

	this->config->text = this->currentText;
	this->timeZone = 0;

	this->config->ntpserver[0] = 129;
	this->config->ntpserver[1] = 6;
	this->config->ntpserver[2] = 15;
	this->config->ntpserver[3] = 28;
	this->ntpserver[0] = this->config->ntpserver[0];
	this->ntpserver[1] = this->config->ntpserver[1];
	this->ntpserver[2] = this->config->ntpserver[2];
	this->ntpserver[3] = this->config->ntpserver[3];
}
void ConfigClass::formatEeprom()
{
	Serial.println("Formating EEPROM");
	// write a 0 to all 4096 bytes of the EEPROM
	for (int i = 0; i < 4096; i++)
		EEPROM.write(i, 0);
	EEPROM.commit();
	
	Serial.println("Done...");
}
// Reads the content of the EEPROM into the EEPROM buffer and copies the values to the
// public member variables. Resets (and saves) the values to their defaults if the
// EEPROM data is not initialized.
void ConfigClass::load()
{
	// this->formatEeprom();
	Serial.println("Reading EEPROM config");

	for (int i = 0; i < EEPROM_SIZE; i++)
		this->eeprom_data[i] = EEPROM.read(i);
	if (this->config->magic != 0xDEADBEEF)
	{
		Serial.println("EEPROM config invalid, writing default values");
		this->reset();
		this->save();
	}

	this->currentAnimation = this->config->animationType;
	this->currentColor = this->config->color;
	this->currentText = this->config->text;

	this->heartbeat = this->config->heartbeat;
	this->delay = this->config->delay;

	this->timeZone = this->config->timeZone;
	for (int i = 0; i < 4; i++)
		this->ntpserver[i] = this->config->ntpserver[i];
}
