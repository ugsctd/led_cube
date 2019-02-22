#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"

//---------------------------------------------------------------------------------------
// global instance
//---------------------------------------------------------------------------------------
ConfigClass Config = ConfigClass();

//---------------------------------------------------------------------------------------
// ConfigClass
//
// Constructor, loads default values
//
// -> --
// <- --
//---------------------------------------------------------------------------------------
ConfigClass::ConfigClass()
{
	this->reset();
}

//---------------------------------------------------------------------------------------
// ~ConfigClass
//
// destructor
//
// -> --
// <- --
//---------------------------------------------------------------------------------------
ConfigClass::~ConfigClass()
{
}

//---------------------------------------------------------------------------------------
// begin
//
// Initializes the class and loads current configuration from EEPROM into class members.
//
// -> --
// <- --
//---------------------------------------------------------------------------------------
void ConfigClass::begin()
{
	EEPROM.begin(EEPROM_SIZE);
	this->load();
}

//---------------------------------------------------------------------------------------
// saveDelayed
//
// Copies the current class member values to EEPROM buffer and writes it to the EEPROM
// after 10 seconds.
//
// -> --
// <- --
//---------------------------------------------------------------------------------------
void ConfigClass::saveDelayed()
{
  this->delayedWriteTimer = 1000; // 10 seconds using 10 ms timer
}

//---------------------------------------------------------------------------------------
// save
//
// Copies the current class member values to EEPROM buffer and writes it to the EEPROM.
//
// -> --
// <- --
//---------------------------------------------------------------------------------------
void ConfigClass::save()
{
  this->delayedWriteFlag = false;
  
	this->config->bg = this->bg;
	this->config->fg = this->fg;
	this->config->s = this->s;
  this->config->brightnessOverride = this->brightnessOverride;
	this->config->timeZone = this->timeZone;
  this->config->dialect = this->dialect;
	this->config->heartbeat = this->heartbeat;
  this->config->esIst = this->esIst;
	// this->config->mode = (uint32_t) this->defaultMode;
	for (int i = 0; i < 4; i++)
		this->config->ntpserver[i] = this->ntpserver[i];

	for (int i = 0; i < EEPROM_SIZE; i++)
		EEPROM.write(i, this->eeprom_data[i]);
	EEPROM.commit();
}

//---------------------------------------------------------------------------------------
// reset
//
// Sets default values in EEPROM buffer and member variables.
//
// -> --
// <- --
//---------------------------------------------------------------------------------------
void ConfigClass::reset()
{
	this->config->magic = 0xDEADBEEF;
	this->config->bg =
	{	0, 0, 0};
	this->bg = this->config->bg;

	this->config->fg =
	{	255, 255, 255};
	this->fg = this->config->fg;

	this->config->s =
	{	0, 0, 0};
	this->s = this->config->s;

	this->config->heartbeat = false;
	this->heartbeat = this->config->heartbeat;

  this->config->esIst = true;
  this->esIst = this->config->esIst;

	// this->defaultMode = DisplayMode::plain;
	// this->config->mode = (uint32_t) this->defaultMode;
	this->timeZone = 0;
 
  this->dialect = 0;
  this->config->dialect = this->dialect;

	this->config->ntpserver[0] = 192;
	this->config->ntpserver[1] = 53;
	this->config->ntpserver[2] = 103;
	this->config->ntpserver[3] = 108;
	this->ntpserver[0] = this->config->ntpserver[0];
	this->ntpserver[1] = this->config->ntpserver[1];
	this->ntpserver[2] = this->config->ntpserver[2];
	this->ntpserver[3] = this->config->ntpserver[3];

  this->config->brightnessTable[0] = 40;
  this->config->brightnessTable[1] = 50;
  this->config->brightnessTable[2] = 60;
  this->config->brightnessTable[3] = 220;
  this->config->brightnessTable[4] = 240;
  this->config->brightnessTable[5] = 255;
  this->brightnessTable[0] = this->config->brightnessTable[0];
  this->brightnessTable[1] = this->config->brightnessTable[1];
  this->brightnessTable[2] = this->config->brightnessTable[2];
  this->brightnessTable[3] = this->config->brightnessTable[3];
  this->brightnessTable[4] = this->config->brightnessTable[4];
  this->brightnessTable[5] = this->config->brightnessTable[5];
}

//---------------------------------------------------------------------------------------
// load
//
// Reads the content of the EEPROM into the EEPROM buffer and copies the values to the
// public member variables. Resets (and saves) the values to their defaults if the
// EEPROM data is not initialized.
//
// -> --
// <- --
//---------------------------------------------------------------------------------------
void ConfigClass::load()
{
	Serial.println("Reading EEPROM config");
	for (int i = 0; i < EEPROM_SIZE; i++)
		this->eeprom_data[i] = EEPROM.read(i);
	if (this->config->magic != 0xDEADBEEF)
	{
		Serial.println("EEPROM config invalid, writing default values");
		this->reset();
		this->save();
	}
	this->bg = this->config->bg;
	this->fg = this->config->fg;
	this->s = this->config->s;
  this->brightnessOverride = this->config->brightnessOverride;
	// this->defaultMode = (DisplayMode) this->config->mode;
	this->heartbeat = this->config->heartbeat;
  this->esIst = this->config->esIst;
	this->timeZone = this->config->timeZone;
  this->dialect = this->config->dialect;
	for (int i = 0; i < 4; i++)
		this->ntpserver[i] = this->config->ntpserver[i];

  for (int i = 0; i < 6; i++)
    this->brightnessTable[i] = this->config->brightnessTable[i];
    
}
