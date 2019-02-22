#include <Arduino.h>
#include "brightness.h"
#include "config.h"

#define PRODUCTION
#ifndef PRODUCTION
  #include "RemoteDebug.h"
  
  extern RemoteDebug Debug;
  
  #define VERBOSE(...)  if (Debug.isActive(Debug.VERBOSE)) { Debug.printf(__VA_ARGS__);}
  #define DEBUG(...)  if (Debug.isActive(Debug.DEBUG)) { Debug.printf(__VA_ARGS__);}
  #define INFO(...)     if (Debug.isActive(Debug.INFO)) { Debug.printf(__VA_ARGS__);}
  #define WARNING(...) if (Debug.isActive(Debug.WARNING)) { Debug.printf(__VA_ARGS__);}
  #define ERROR(...) if (Debug.isActive(Debug.ERROR)) { Debug.printf(__VA_ARGS__);}
#endif


//---------------------------------------------------------------------------------------
// global instance
//---------------------------------------------------------------------------------------
BrightnessClass Brightness = BrightnessClass();

//---------------------------------------------------------------------------------------
// BrightnessClass
//
// Constructor, initializes filter with current ADC value
//
// -> --
// <- --
//---------------------------------------------------------------------------------------
BrightnessClass::BrightnessClass()
{
	this->avg = analogRead(A0);
}

//---------------------------------------------------------------------------------------
// filter
//
// Exponential moving average filter using integer arithmetics. Implements the
// following behaviour:
//
//   output = (1-coeff)*last_output + coeff*input
//
// FILTER_COEFFICIENT is defined as fixed point number with 16.16 bits resolution.
//
// -> input: filter input [0...65535]
// <- --
//---------------------------------------------------------------------------------------
uint32_t BrightnessClass::filter(uint16_t input)
{
	uint32_t tmp = (65536 - FILTER_COEFFICIENT) * this->avg
			     + FILTER_COEFFICIENT * (uint32_t)input;
	return (tmp + 32768) >> 16;
}

//---------------------------------------------------------------------------------------
// getBrightnessForADCValue
//
// Calculates a brightness value for a given ADC value using a table and interpolation.
//
// -> adcValue: input value [0...1023]
// <- brightness [0...256]
//---------------------------------------------------------------------------------------
uint32_t BrightnessClass::getBrightnessForADCValue(uint32_t adcValue)
{
	float lightTable[] = {
		0, Config.brightnessTable[0],
		59, Config.brightnessTable[1],
		67, Config.brightnessTable[2],
		120, Config.brightnessTable[3],
		300, Config.brightnessTable[4],
		1023, Config.brightnessTable[5],
		-1, -1
	};

//	float lightTable[] = {
//		0, 160,
//		100, 96,
//		200, 64,
//		300, 32,
//		400, 15,
//		1023, 15,
//		-1, -1
//	};

  #ifndef PRODUCTION
    VERBOSE("Sizeof lightTable: %d\r\n",sizeof(lightTable)/sizeof(*lightTable));
    VERBOSE("Sizeof brightnessTable: %d\r\n",sizeof(Config.brightnessTable)/sizeof(*Config.brightnessTable));
  #endif
  
  
	if(Config.brightnessOverride<256)
	{
		return Config.brightnessOverride;
	}

  //adcValue = 900;
	int i = 0;
	while (lightTable[i] != -1)
	{
		if (adcValue >= lightTable[i] && adcValue < lightTable[i + 2])
		{
			float x1 = lightTable[i + 0];
			float y1 = lightTable[i + 1];
			float x2 = lightTable[i + 2];
			float y2 = lightTable[i + 3];
			float result = y1 + ((float)adcValue - x1) * (y2 - y1) / (x2 - x1);
			return (int) result;
		}
		i += 2;
	}

	return 0;
}

//---------------------------------------------------------------------------------------
// value
//
// Triggers a new sample to be read from light sensor, returns value from average filter
//
// -> --
// <- low pass filtered brightness value
//---------------------------------------------------------------------------------------
uint32_t BrightnessClass::value()
{
  
  #ifndef PRODUCTION
    VERBOSE("BrightnessClass::value() started");
  #endif
  
  
  if(Config.brightnessOverride<256)
  {
//  VERBOSE("Fasttrack without analogRead brightness=%i\r\n",brightnessOverride);
    return Config.brightnessOverride;
  }
	// read next ADC value and apply filter
	this->avg = this->filter(analogRead(A0));

	// calculate brightness value for filtered ADC value
	uint32_t brightness = this->getBrightnessForADCValue(this->avg);

  #ifndef PRODUCTION
    VERBOSE("BrightnessClass::value() ended");
  #endif
	return brightness;
}
