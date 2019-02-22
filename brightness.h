#ifndef _BRIGHTNESS_H_
#define _BRIGHTNESS_H_

#include <stdint.h>

#define ALPHA(x) ((uint16_t)(x * 65535.0f))
#define FILTER_COEFFICIENT ALPHA(0.01)

class BrightnessClass
{
public:
	BrightnessClass();
	uint32_t value();

	uint32_t avg = 0;

private:
	uint32_t getBrightnessForADCValue(uint32_t adcValue);
	uint32_t filter(uint16_t input);
};

extern BrightnessClass Brightness;

#endif

