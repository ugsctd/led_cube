// // ESP8266 Wordclock
// // Copyright (C) 2016 Thoralt Franz, https://github.com/thoralt
// //
// //  This module implements functions to manage the WS2812B LEDs. Two buffers contain
// //  color information with current state and fade target state and are updated by
// //  either simple set operations or integrated screensavers (matrix, stars, heart).
// //  Also contains part of the data and logic for the hourglass animation.
// //
// // This program is free software: you can redistribute it and/or modify
// // it under the terms of the GNU General Public License as published by
// // the Free Software Foundation, either version 3 of the License, or
// // (at your option) any later version.
// //
// // This program is distributed in the hope that it will be useful,
// // but WITHOUT ANY WARRANTY; without even the implied warranty of
// // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// // GNU General Public License for more details.
// //
// // You should have received a copy of the GNU General Public License
// // along with this program.  If not, see <http://www.gnu.org/licenses/>.
// #include "ledfunctions.h"


// #define PRODUCTION
// #ifndef PRODUCTION
//   #include "RemoteDebug.h"
  
//   extern RemoteDebug Debug;
  
//   #define VERBOSE(...)  if (Debug.isActive(Debug.VERBOSE)) { Debug.printf(__VA_ARGS__);}
//   #define DEBUG(...)  if (Debug.isActive(Debug.DEBUG)) { Debug.printf(__VA_ARGS__);}
//   #define INFO(...)     if (Debug.isActive(Debug.INFO)) { Debug.printf(__VA_ARGS__);}
//   #define WARNING(...) if (Debug.isActive(Debug.WARNING)) { Debug.printf(__VA_ARGS__);}
//   #define ERROR(...) if (Debug.isActive(Debug.ERROR)) { Debug.printf(__VA_ARGS__);}
// #endif

// //---------------------------------------------------------------------------------------
// #if 1 // variables
// //---------------------------------------------------------------------------------------

// //---------------------------------------------------------------------------------------
// // global instance
// //---------------------------------------------------------------------------------------
// LEDFunctionsClass LED = LEDFunctionsClass();

// uint8_t fireBuf[NUM_PIXELS];
// uint8_t plasmaBuf[NUM_PIXELS];

// #endif
// //---------------------------------------------------------------------------------------
// #if 1 // getters, setters, data flow
// //---------------------------------------------------------------------------------------

// //---------------------------------------------------------------------------------------
// // ~LEDFunctionsClass
// //
// // Destructor
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// LEDFunctionsClass::~LEDFunctionsClass()
// {
// //	for(MatrixObject* m : this->matrix_objects) delete m;
// }

// //---------------------------------------------------------------------------------------
// // LEDFunctionsClass
// //
// // Constructor, initializes data structures
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// LEDFunctionsClass::LEDFunctionsClass()
// {
// 	this->changeDialect(Config.dialect);
// 	// initialize matrix objects with random coordinates
// 	//for (int i = 0; i < NUM_MATRIX_OBJECTS; i++)
// 	//{
// 	//	this->matrix.push_back(MatrixObject());
// 	//}

// 	// initialize star objects with default coordinates
// 	//for (int i = 0; i < NUM_STARS; i++) this->stars.push_back(StarObject());

// 	// set random coordinates with minimum distance to other star objects
// 	//for (StarObject& s : this->stars) s.randomize(this->stars);
// }

// //---------------------------------------------------------------------------------------
// // begin
// //
// // Initializes the LED driver
// //
// // -> pin: hardware pin to use for WS2812B data output
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::begin(int pin)
// {
	
// }

// //---------------------------------------------------------------------------------------
// // process
// //
// // Drives internal data flow, should be called repeatedly from main loop()
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::process()
// {
// 	if(Config.debugMode) return;

// 	// check time values against boundaries
// 	if(this->h > 23 || this->h < 0) this->h = 0;
// 	if(this->m > 59 || this->m < 0) this->m = 0;
// 	if(this->s > 59 || this->s < 0) this->s = 0;
// 	if(this->ms > 999 || this->ms < 0) this->ms = 0;

// 	// load palette colors from configuration
// 	palette_entry palette[] = {
// 		{Config.bg.r, Config.bg.g, Config.bg.b},
// 		{Config.fg.r, Config.fg.g, Config.fg.b},
// 		{Config.s.r,  Config.s.g,  Config.s.b}};
// 	uint8_t buf[NUM_PIXELS];

// 	switch(this->mode)
// 	{
// 	case DisplayMode::wifiManager:
// 		this->renderWifiManager();
// 		break;
// 	case DisplayMode::yellowHourglass:
// 		this->renderHourglass(Config.hourglassState, false);
// 		break;
// 	case DisplayMode::greenHourglass:
// 		this->renderHourglass(Config.hourglassState, true);
// 		break;
// 	case DisplayMode::update:
// 		this->renderUpdate();
// 		break;
// 	case DisplayMode::updateComplete:
// 		this->renderUpdateComplete();
// 		break;
// 	case DisplayMode::updateError:
// 		this->renderUpdateError();
// 		break;
// 	case DisplayMode::red:
// 		this->renderRed();
// 		break;
// 	case DisplayMode::green:
// 		this->renderGreen();
// 		break;
// 	case DisplayMode::blue:
// 		this->renderBlue();
// 		break;
// 	case DisplayMode::flyingLettersVerticalUp:
// 	case DisplayMode::flyingLettersVerticalDown:
// 		this->renderFlyingLetters();
// 		break;
// 	case DisplayMode::explode:
// 		this->renderExplosion();
// 		break;
// 	case DisplayMode::matrix:
// 		this->renderMatrix();
// 		break;
// 	case DisplayMode::heart:
// 		this->renderHeart();
// 		break;
// 	case DisplayMode::stars:
// 		this->renderStars();
// 		break;
//   case DisplayMode::fire:
//     this->renderFire();
//     break;
//   case DisplayMode::plasma:
//     this->renderPlasma();
//     break;
// 	case DisplayMode::fade:
// 		this->renderTime(buf, this->h, this->m, this->s, this->ms);
// 		this->set(buf, palette, false);
// 		this->fade();
// 		break;

// 	case DisplayMode::random: // TODO: Implement random display mode
// 	case DisplayMode::plain:
// 	default:
// 		this->renderTime(buf, this->h, this->m, this->s, this->ms);
// 		this->set(buf, palette, true);
// 		break;
// 	}

// 	// transfer this->currentValues to LEDs
// 	this->show();
// }

// //---------------------------------------------------------------------------------------
// // setBrightness
// //
// // Sets the brightness for the WS2812 values. Will be multiplied with each color
// // component when sending data to WS2812.
// //
// // -> brightness: [0...256]
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::setBrightness(int brightness)
// {
// 	this->brightness = brightness;
// }

// //---------------------------------------------------------------------------------------
// // setTime
// //
// // Sets the time which will be used to drive the LED matrix. The internal time is _not_
// // updated, this function must be called repeatedly if the time changes.
// //
// // -> h, m, s, ms: Time in hours, minutes, seconds, milliseconds
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::setTime(int h, int m, int s, int ms)
// {
// 	this->h = h;
// 	this->m = m;
// 	this->s = s;
// 	this->ms = ms;
// }

// //---------------------------------------------------------------------------------------
// // setMode
// //
// // Sets the display mode to one of the members of the DisplayMode enum and thus changes
// // what will be shown on the display during the next calls of LEDFunctionsClass.process()
// //
// // -> newMode: mode to be set
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::setMode(DisplayMode newMode)
// {
// 	uint8_t buf[NUM_PIXELS];
// 	DisplayMode previousMode = this->mode;
// 	this->mode = newMode;

// 	// if we changed to an animated letters mode, then start animation
// 	// even if the current time did not yet change
// 	if(newMode != previousMode &&
// 			(newMode == DisplayMode::flyingLettersVerticalUp ||
// 			newMode == DisplayMode::flyingLettersVerticalDown))
// 	{
// 		this->renderTime(buf, this->h, this->m, this->s, this->ms);
// 		// this->prepareFlyingLetters(buf);
// 	}

// 	// if we changed to exploding letters mode, then start animation
// 	// even if the current time did not yet change
// 	if(newMode != previousMode && newMode == DisplayMode::explode)
// 	{
// 		this->renderTime(buf, this->h, this->m, this->s, this->ms);
// 		// this->prepareExplosion(buf);
// 	}

// 	this->process();
// }

// //---------------------------------------------------------------------------------------
// // set
// //
// // Sets the internal LED buffer to new values based on an indexed source buffer and an
// // associated palette. Does not display colors immediately, fades to new colors instead.
// //
// // Attention: If buf is PROGMEM, make sure it is aligned at 32 bit and its size is
// // a multiple of 4 bytes!
// //
// // -> buf: indexed source buffer
// //	palette: color definition for source buffer
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::set(const uint8_t *buf, palette_entry palette[])
// {
// 	this->set(buf, palette, false);
// }

// //---------------------------------------------------------------------------------------
// // set
// //
// // Sets the internal LED buffer to new values based on an indexed source buffer and an
// // associated palette.
// //
// // Attention: If buf is PROGMEM, make sure it is aligned at 32 bit and its size is
// // a multiple of 4 bytes!
// //
// // -> buf: indexed source buffer
// //	  palette: color definition for source buffer
// //	  immediately: if true, display buffer immediately; fade to new colors if false
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::set(const uint8_t *buf, palette_entry palette[],
// 		bool immediately)
// {
// 	this->setBuffer(this->targetValues, buf, palette);

// 	if (immediately)
// 	{
// 		this->setBuffer(this->currentValues, buf, palette);
// 	}
// }

// //---------------------------------------------------------------------------------------
// // setBuffer
// //
// // Fills a buffer (e. g. this->targetValues) with color data based on indexed source
// // pixels and a palette. Pays attention to 32 bit boundaries, so use with PROGMEM is
// // safe.
// //
// // -> target: color buffer, e. g. this->targetValues or this->currentValues
// //    source: buffer with color indexes
// //	  palette: colors for indexed source buffer
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::setBuffer(uint8_t *target, const uint8_t *source,
// 		palette_entry palette[])
// {
//   uint8_t mapping;
// 	uint32_t palette_index, curveOffset;

// 	// cast source to 32 bit pointer to ensure 32 bit aligned access
// 	uint32_t *buf = (uint32_t*) source;
// 	// this holds the current 4 bytes
// 	uint32_t currentDWord;
// 	// this is a pointer to the current 4 bytes for access as single bytes
// 	uint8_t *currentBytes = (uint8_t*) &currentDWord;
// 	// this counts bytes from 0...3
// 	uint32_t byteCounter = 0;
// 	for (int i = 0; i < NUM_PIXELS; i++)
// 	{
// 		// get next 4 bytes
// 		if (byteCounter == 0) currentDWord = buf[i >> 2];

// 		palette_index = currentBytes[byteCounter];
// 		// mapping = pgm_read_byte(LEDFunctionsClass::mapping + i) * 3;
// 		// curveOffset = pgm_read_byte(LEDFunctionsClass::brightnessCurveSelect + i) << 8;

// 		// select color value using palette and brightness correction curves
// 		// target[mapping + 0] = pgm_read_byte(brightnessCurvesR + (curveOffset + palette[palette_index].r));
// 		// target[mapping + 1] = pgm_read_byte(brightnessCurvesG + (curveOffset + palette[palette_index].g));
// 		// target[mapping + 2] = pgm_read_byte(brightnessCurvesB + (curveOffset + palette[palette_index].b));

// 		byteCounter = (byteCounter + 1) & 0x03;
// 	}
// }

// //---------------------------------------------------------------------------------------
// // fade
// //
// // Fade one step of the color values from this->currentValues[i] to 
// // this->targetValues[i]. Uses non-linear fade speed depending on distance to target
// // value.
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::fade()
// {
// 	static int prescaler = 0;
// 	if(++prescaler<2) return;
// 	prescaler = 0;

// 	int delta;
// 	for (int i = 0; i < NUM_PIXELS * 3; i++)
// 	{
// 		delta = this->targetValues[i] - this->currentValues[i];
// 		if (delta > 64) this->currentValues[i] += 8;
// 		else if (delta > 16) this->currentValues[i] += 4;
// 		else if (delta > 0) this->currentValues[i]++;
// 		else if (delta < -64) this->currentValues[i] -= 8;
// 		else if (delta < -16) this->currentValues[i] -= 4;
// 		else if (delta < 0) this->currentValues[i]--;
// 	}
// }

// //---------------------------------------------------------------------------------------
// // show
// //
// // Internal method, copies this->currentValues to WS2812 object while applying brightness
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::show()
// {
// 	uint8_t *data = this->currentValues;
// 	int ofs = 0;

// 	// copy current color values to LED object and display it
// 	// for (int i = 0; i < NUM_PIXELS; i++)
// 	// {
// 	// 	// this->pixels->SetPixelColor(i,
// 	// 	// 		RgbColor(((int) data[ofs + 0] * this->brightness) >> 8,
// 	// 	// 				      ((int) data[ofs + 1] * this->brightness) >> 8,
// 	// 	// 				      ((int) data[ofs + 2] * this->brightness) >> 8));
// 	// 	// ofs += 3;
// 	// }
// 	// this->pixels->Show();
// }

// //---------------------------------------------------------------------------------------
// // getOffset
// //
// // Calculates the offset of a given RGB triplet inside the LED buffer.
// // Does range checking for x and y, uses the internal mapping table.
// //
// // -> x: x coordinate
// //    y: y coordinate
// // <- offset for given coordinates
// //---------------------------------------------------------------------------------------
// int LEDFunctionsClass::getOffset(int x, int y)
// {
// 	if (x>=0 && y>=0 && x<LEDFunctionsClass::width && y<LEDFunctionsClass::height)
// 	{
// 		// return pgm_read_byte(LEDFunctionsClass::mapping + (x + y*LEDFunctionsClass::width)) * 3;
// 	}
// 	else
// 	{
// 		return 0;
// 	}
// }

// //---------------------------------------------------------------------------------------
// // fillBackground
// //
// // Initializes the buffer with either background (=0) or seconds progress (=2),
// // part of the background will be illuminated with color 2 depending on current
// // seconds/milliseconds value, whole screen is backlit when seconds = 59
// //
// // -> seconds, milliseconds: Time value which the fill process will base on
// //    buf: destination buffer
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::fillBackground(int seconds, int milliseconds, uint8_t *buf)
// {
// 	int pos = (((seconds * 1000 + milliseconds) * 110) / 60000) + 1;
// 	for (int i = 0; i < NUM_PIXELS; i++) buf[i] = (i < pos) ? 2 : 0;
// }

// #endif

// //---------------------------------------------------------------------------------------
// #if 1 // rendering methods
// //---------------------------------------------------------------------------------------

// //---------------------------------------------------------------------------------------
// // renderTime
// //
// // Loads internal buffers with the current time representation
// //
// // -> target: If NULL, a local buffer is used and time is actually being displayed
// //            if not null, time is not being displayed, but the given buffer is being
// //            filled with palette indexes representing the time
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::renderTime(uint8_t *target, int h, int m, int s, int ms)
// {
// 	this->fillBackground(s, ms, target);

//   if (Config.esIst == true){
//    	// set static LEDs if desired
//   	target[0] = 1; // E
//   	target[1] = 1; // S
  
//   	target[3] = 1; // I
//   	target[4] = 1; // S
//   	target[5] = 1; // T
//   }
// 	// minutes 1...4 for the corners
// 	for(int i=0; i<=((m%5)-1); i++) target[10 * 11 + i] = 1;

// 	// iterate over minutes_template
// 	int adjust_hour = 0;
// 	for(leds_template_t t : LEDFunctionsClass::minutesTemplate)
// 	{
// 		// test if this template matches the current minute
// 		if(m >= t.param1 && m <= t.param2)
// 		{
// 			// set all LEDs defined in this template
// 			for(int i : t.LEDs) target[i] = 1;
// 			adjust_hour = t.param0;
// 			break;
// 		}
// 	}

// 	// adjust hour display if necessary (e. g. 09:45 = quarter to *TEN* instead of NINE)
// 	h += adjust_hour;
// 	if(h > 23)	h -= 24;

// 	// iterate over hours template
// 	for(leds_template_t t : LEDFunctionsClass::hoursTemplate)
// 	{
// 		// test if this template matches the current hour
// 		if((t.param1 == h || t.param2 == h) &&
// 		   ((t.param0 == 1 && m < 5)  || // special case full hour
// 			(t.param0 == 2 && m >= 5) || // special case hour + minutes
// 			(t.param0 == 0)))            // normal case
// 		{
// 			// set all LEDs defined in this template
// 			for(int i : t.LEDs) target[i] = 1;
// 			break;
// 		}
// 	}

// 	// DEBUG
//   #ifndef PRODUCTION

//   if (Debug.isActive(Debug.DEBUG)){
//     static int last_minutes = -1;
//     if(last_minutes != this->m)
//     {
//       last_minutes = this->m;
//       Debug.printf("h=%i, m=%i, s=%i\r\n", this->h, this->m, this->s);
//       for(int y=0; y<10; y++)
//       {
//         for(int x=0; x<11; x++)
//         {
//           Debug.print(target[y*11+x]);
//           Debug.print(' ');
//         }
//         Debug.println(' ');
//       }
//     }
//   }
  
//   #endif

// }

// //---------------------------------------------------------------------------------------
// // renderHourglass
// //
// // Immediately displays an animation step of the hourglass animation.
// // ATTENTION: Animation frames must start at 32 bit boundary each!
// //
// // -> animationStep: Number of current frame [0...HOURGLASS_ANIMATION_FRAMES]
// //    green: Flag to switch the palette color 3 to green instead of yellow (used in the
// //           second half of the hourglass animation to indicate the short wait-for-OTA
// //           window)
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::renderHourglass(uint8_t animationStep, bool green)
// {
// 	// colors in palette: black, white, yellow
// 	palette_entry p[] = {{0, 0, 0}, {255, 255, 255}, {255, 255, 0}, {255, 255, 0}};

// 	// delete red component in palette entry 3 to make this color green
// 	if(green) p[3].r = 0;

// 	if (animationStep >= HOURGLASS_ANIMATION_FRAMES) animationStep = 0;
// 	this->set(hourglass_animation[animationStep], p, true);
// }

// //---------------------------------------------------------------------------------------
// // renderMatrix
// //
// // Renders one frame of the matrix animation and displays it immediately
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::renderMatrix()
// {
// 	// clear buffer
// 	memset(this->currentValues, 0, sizeof(this->currentValues));

// 	// sort by y coordinate for correct overlapping
// 	std::sort(matrix.begin(), matrix.end());

// 	// iterate over all matrix objects, move and render them
// 	for (MatrixObject &m : this->matrix) m.render(this->currentValues);
// }

// const palette_entry LEDFunctionsClass::firePalette[256] = {
//    {0, 0, 0}, {4, 0, 0}, {8, 0, 0}, {12, 0, 0}, {16, 0, 0}, {20, 0, 0}, {24, 0, 0}, {28, 0, 0},
//    {32, 0, 0}, {36, 0, 0}, {40, 0, 0}, {44, 0, 0}, {48, 0, 0}, {52, 0, 0}, {56, 0, 0}, {60, 0, 0},
//    {64, 0, 0}, {68, 0, 0}, {72, 0, 0}, {76, 0, 0}, {80, 0, 0}, {85, 0, 0}, {89, 0, 0}, {93, 0, 0},
//    {97, 0, 0}, {101, 0, 0}, {105, 0, 0}, {109, 0, 0}, {113, 0, 0}, {117, 0, 0}, {121, 0, 0}, {125, 0, 0},
//    {129, 0, 0}, {133, 0, 0}, {137, 0, 0}, {141, 0, 0}, {145, 0, 0}, {149, 0, 0}, {153, 0, 0}, {157, 0, 0},
//    {161, 0, 0}, {165, 0, 0}, {170, 0, 0}, {174, 0, 0}, {178, 0, 0}, {182, 0, 0}, {186, 0, 0}, {190, 0, 0},
//    {194, 0, 0}, {198, 0, 0}, {202, 0, 0}, {206, 0, 0}, {210, 0, 0}, {214, 0, 0}, {218, 0, 0}, {222, 0, 0},
//    {226, 0, 0}, {230, 0, 0}, {234, 0, 0}, {238, 0, 0}, {242, 0, 0}, {246, 0, 0}, {250, 0, 0}, {255, 0, 0},
//    {255, 0, 0}, {255, 8, 0}, {255, 16, 0}, {255, 24, 0}, {255, 32, 0}, {255, 41, 0}, {255, 49, 0}, {255, 57, 0},
//    {255, 65, 0}, {255, 74, 0}, {255, 82, 0}, {255, 90, 0}, {255, 98, 0}, {255, 106, 0}, {255, 115, 0}, {255, 123, 0},
//    {255, 131, 0}, {255, 139, 0}, {255, 148, 0}, {255, 156, 0}, {255, 164, 0}, {255, 172, 0}, {255, 180, 0}, {255, 189, 0},
//    {255, 197, 0}, {255, 205, 0}, {255, 213, 0}, {255, 222, 0}, {255, 230, 0}, {255, 238, 0}, {255, 246, 0}, {255, 255, 0},
//    {255, 255, 0}, {255, 255, 1}, {255, 255, 3}, {255, 255, 4}, {255, 255, 6}, {255, 255, 8}, {255, 255, 9}, {255, 255, 11},
//    {255, 255, 12}, {255, 255, 14}, {255, 255, 16}, {255, 255, 17}, {255, 255, 19}, {255, 255, 20}, {255, 255, 22}, {255, 255, 24},
//    {255, 255, 25}, {255, 255, 27}, {255, 255, 28}, {255, 255, 30}, {255, 255, 32}, {255, 255, 33}, {255, 255, 35}, {255, 255, 36},
//    {255, 255, 38}, {255, 255, 40}, {255, 255, 41}, {255, 255, 43}, {255, 255, 44}, {255, 255, 46}, {255, 255, 48}, {255, 255, 49},
//    {255, 255, 51}, {255, 255, 52}, {255, 255, 54}, {255, 255, 56}, {255, 255, 57}, {255, 255, 59}, {255, 255, 60}, {255, 255, 62},
//    {255, 255, 64}, {255, 255, 65}, {255, 255, 67}, {255, 255, 68}, {255, 255, 70}, {255, 255, 72}, {255, 255, 73}, {255, 255, 75},
//    {255, 255, 76}, {255, 255, 78}, {255, 255, 80}, {255, 255, 81}, {255, 255, 83}, {255, 255, 85}, {255, 255, 86}, {255, 255, 88},
//    {255, 255, 89}, {255, 255, 91}, {255, 255, 93}, {255, 255, 94}, {255, 255, 96}, {255, 255, 97}, {255, 255, 99}, {255, 255, 101},
//    {255, 255, 102}, {255, 255, 104}, {255, 255, 105}, {255, 255, 107}, {255, 255, 109}, {255, 255, 110}, {255, 255, 112}, {255, 255, 113},
//    {255, 255, 115}, {255, 255, 117}, {255, 255, 118}, {255, 255, 120}, {255, 255, 121}, {255, 255, 123}, {255, 255, 125}, {255, 255, 126},
//    {255, 255, 128}, {255, 255, 129}, {255, 255, 131}, {255, 255, 133}, {255, 255, 134}, {255, 255, 136}, {255, 255, 137}, {255, 255, 139},
//    {255, 255, 141}, {255, 255, 142}, {255, 255, 144}, {255, 255, 145}, {255, 255, 147}, {255, 255, 149}, {255, 255, 150}, {255, 255, 152},
//    {255, 255, 153}, {255, 255, 155}, {255, 255, 157}, {255, 255, 158}, {255, 255, 160}, {255, 255, 161}, {255, 255, 163}, {255, 255, 165},
//    {255, 255, 166}, {255, 255, 168}, {255, 255, 170}, {255, 255, 171}, {255, 255, 173}, {255, 255, 174}, {255, 255, 176}, {255, 255, 178},
//    {255, 255, 179}, {255, 255, 181}, {255, 255, 182}, {255, 255, 184}, {255, 255, 186}, {255, 255, 187}, {255, 255, 189}, {255, 255, 190},
//    {255, 255, 192}, {255, 255, 194}, {255, 255, 195}, {255, 255, 197}, {255, 255, 198}, {255, 255, 200}, {255, 255, 202}, {255, 255, 203},
//    {255, 255, 205}, {255, 255, 206}, {255, 255, 208}, {255, 255, 210}, {255, 255, 211}, {255, 255, 213}, {255, 255, 214}, {255, 255, 216},
//    {255, 255, 218}, {255, 255, 219}, {255, 255, 221}, {255, 255, 222}, {255, 255, 224}, {255, 255, 226}, {255, 255, 227}, {255, 255, 229},
//    {255, 255, 230}, {255, 255, 232}, {255, 255, 234}, {255, 255, 235}, {255, 255, 237}, {255, 255, 238}, {255, 255, 240}, {255, 255, 242},
//    {255, 255, 243}, {255, 255, 245}, {255, 255, 246}, {255, 255, 248}, {255, 255, 250}, {255, 255, 251}, {255, 255, 253}, {255, 255, 255}
//  };
//  const palette_entry LEDFunctionsClass::plasmaPalette[256] = {
//    {255, 0, 0}, {255, 6, 0}, {255, 12, 0}, {255, 18, 0}, {255, 24, 0}, {255, 30, 0}, {255, 36, 0}, {255, 42, 0},
//    {255, 48, 0}, {255, 54, 0}, {255, 60, 0}, {255, 66, 0}, {255, 72, 0}, {255, 78, 0}, {255, 84, 0}, {255, 90, 0},
//    {255, 96, 0}, {255, 102, 0}, {255, 108, 0}, {255, 114, 0}, {255, 120, 0}, {255, 126, 0}, {255, 131, 0}, {255, 137, 0},
//    {255, 143, 0}, {255, 149, 0}, {255, 155, 0}, {255, 161, 0}, {255, 167, 0}, {255, 173, 0}, {255, 179, 0}, {255, 185, 0},
//    {255, 191, 0}, {255, 197, 0}, {255, 203, 0}, {255, 209, 0}, {255, 215, 0}, {255, 221, 0}, {255, 227, 0}, {255, 233, 0},
//    {255, 239, 0}, {255, 245, 0}, {255, 251, 0}, {253, 255, 0}, {247, 255, 0}, {241, 255, 0}, {235, 255, 0}, {229, 255, 0},
//    {223, 255, 0}, {217, 255, 0}, {211, 255, 0}, {205, 255, 0}, {199, 255, 0}, {193, 255, 0}, {187, 255, 0}, {181, 255, 0},
//    {175, 255, 0}, {169, 255, 0}, {163, 255, 0}, {157, 255, 0}, {151, 255, 0}, {145, 255, 0}, {139, 255, 0}, {133, 255, 0},
//    {128, 255, 0}, {122, 255, 0}, {116, 255, 0}, {110, 255, 0}, {104, 255, 0}, {98, 255, 0}, {92, 255, 0}, {86, 255, 0},
//    {80, 255, 0}, {74, 255, 0}, {68, 255, 0}, {62, 255, 0}, {56, 255, 0}, {50, 255, 0}, {44, 255, 0}, {38, 255, 0},
//    {32, 255, 0}, {26, 255, 0}, {20, 255, 0}, {14, 255, 0}, {8, 255, 0}, {2, 255, 0}, {0, 255, 4}, {0, 255, 10},
//    {0, 255, 16}, {0, 255, 22}, {0, 255, 28}, {0, 255, 34}, {0, 255, 40}, {0, 255, 46}, {0, 255, 52}, {0, 255, 58},
//    {0, 255, 64}, {0, 255, 70}, {0, 255, 76}, {0, 255, 82}, {0, 255, 88}, {0, 255, 94}, {0, 255, 100}, {0, 255, 106},
//    {0, 255, 112}, {0, 255, 118}, {0, 255, 124}, {0, 255, 129}, {0, 255, 135}, {0, 255, 141}, {0, 255, 147}, {0, 255, 153},
//    {0, 255, 159}, {0, 255, 165}, {0, 255, 171}, {0, 255, 177}, {0, 255, 183}, {0, 255, 189}, {0, 255, 195}, {0, 255, 201},
//    {0, 255, 207}, {0, 255, 213}, {0, 255, 219}, {0, 255, 225}, {0, 255, 231}, {0, 255, 237}, {0, 255, 243}, {0, 255, 249},
//    {0, 255, 255}, {0, 249, 255}, {0, 243, 255}, {0, 237, 255}, {0, 231, 255}, {0, 225, 255}, {0, 219, 255}, {0, 213, 255},
//    {0, 207, 255}, {0, 201, 255}, {0, 195, 255}, {0, 189, 255}, {0, 183, 255}, {0, 177, 255}, {0, 171, 255}, {0, 165, 255},
//    {0, 159, 255}, {0, 153, 255}, {0, 147, 255}, {0, 141, 255}, {0, 135, 255}, {0, 129, 255}, {0, 124, 255}, {0, 118, 255},
//    {0, 112, 255}, {0, 106, 255}, {0, 100, 255}, {0, 94, 255}, {0, 88, 255}, {0, 82, 255}, {0, 76, 255}, {0, 70, 255},
//    {0, 64, 255}, {0, 58, 255}, {0, 52, 255}, {0, 46, 255}, {0, 40, 255}, {0, 34, 255}, {0, 28, 255}, {0, 22, 255},
//    {0, 16, 255}, {0, 10, 255}, {0, 4, 255}, {2, 0, 255}, {8, 0, 255}, {14, 0, 255}, {20, 0, 255}, {26, 0, 255},
//    {32, 0, 255}, {38, 0, 255}, {44, 0, 255}, {50, 0, 255}, {56, 0, 255}, {62, 0, 255}, {68, 0, 255}, {74, 0, 255},
//    {80, 0, 255}, {86, 0, 255}, {92, 0, 255}, {98, 0, 255}, {104, 0, 255}, {110, 0, 255}, {116, 0, 255}, {122, 0, 255},
//    {128, 0, 255}, {133, 0, 255}, {139, 0, 255}, {145, 0, 255}, {151, 0, 255}, {157, 0, 255}, {163, 0, 255}, {169, 0, 255},
//    {175, 0, 255}, {181, 0, 255}, {187, 0, 255}, {193, 0, 255}, {199, 0, 255}, {205, 0, 255}, {211, 0, 255}, {217, 0, 255},
//    {223, 0, 255}, {229, 0, 255}, {235, 0, 255}, {241, 0, 255}, {247, 0, 255}, {253, 0, 255}, {255, 0, 251}, {255, 0, 245},
//    {255, 0, 239}, {255, 0, 233}, {255, 0, 227}, {255, 0, 221}, {255, 0, 215}, {255, 0, 209}, {255, 0, 203}, {255, 0, 197},
//    {255, 0, 191}, {255, 0, 185}, {255, 0, 179}, {255, 0, 173}, {255, 0, 167}, {255, 0, 161}, {255, 0, 155}, {255, 0, 149},
//    {255, 0, 143}, {255, 0, 137}, {255, 0, 131}, {255, 0, 126}, {255, 0, 120}, {255, 0, 114}, {255, 0, 108}, {255, 0, 102},
//    {255, 0, 96}, {255, 0, 90}, {255, 0, 84}, {255, 0, 78}, {255, 0, 72}, {255, 0, 66}, {255, 0, 60}, {255, 0, 54},
//    {255, 0, 48}, {255, 0, 42}, {255, 0, 36}, {255, 0, 30}, {255, 0, 24}, {255, 0, 18}, {255, 0, 12}, {255, 0, 6}
//  };
 
 
//  double _time = 0;
//  void LEDFunctionsClass::renderPlasma()
//  {
//      int color;
//      double cx, cy, xx, yy;
 
//      _time += 0.05;
 
//      for (int y=0; y<LEDFunctionsClass::height; y++)
//      {
//          yy = (double)y / (double)LEDFunctionsClass::height / 3.0;
//          for (int x=0; x<LEDFunctionsClass::width; x++)
//          {
//              xx = (double)x / (double)LEDFunctionsClass::width / 3.0;
//              cx = xx + 0.5 * sin(_time / 5.0);
//              cy = (double)y/(double)LEDFunctionsClass::height / 3.0 + 0.5 * sin(_time / 3.0);
//              color = (
//                sin(
//                      sqrt(100 * (cx*cx + cy*cy) + 1 + _time) +
//                      6.0 * (xx * sin(_time/2) + yy * cos(_time/3) + _time / 4.0)
//                  ) + 1.0
//        ) * 128.0;
//              plasmaBuf[x + y * LEDFunctionsClass::width] = color;
//          }
//      }
//      this->set(plasmaBuf, (palette_entry*)plasmaPalette, true);
//  }
 
//  void LEDFunctionsClass::renderFire()
//  {
//      int f;
 
//      // iterate over bottom row, create fire seed
//      for (int i = 0; i < LEDFunctionsClass::width; i++)
//      {
//          // only set hot spot with probability of 1/4
//          f = (random(4) == 0) ? random(256) : 0;
 
//          // update one pixel in bottom row
//          fireBuf[i + (LEDFunctionsClass::height - 1) * LEDFunctionsClass::width] = f;
//      }
 
//      int y1, y2, l, r;
//      for (int y = 0; y < LEDFunctionsClass::height - 1; y++)
//      {
//          y1 = y + 1; if (y1 >= LEDFunctionsClass::height) y1 = LEDFunctionsClass::height - 1;
//          y2 = y + 2; if (y2 >= LEDFunctionsClass::height) y2 = LEDFunctionsClass::height - 1;
//          for (int x = 0; x < LEDFunctionsClass::width; x++)
//          {
//              l = x - 1; if (l < 0) l = 0;
//              r = x + 1; if (r >= LEDFunctionsClass::width) r = LEDFunctionsClass::width - 1;
//              fireBuf[x + y * LEDFunctionsClass::width] =
//                  ((fireBuf[y1 * LEDFunctionsClass::width + l]
//                  + fireBuf[y1 * LEDFunctionsClass::width + x]
//                  + fireBuf[y1 * LEDFunctionsClass::width + r]
//                  + fireBuf[y2 * LEDFunctionsClass::width + x])
//                  * 32) / 129;
//          }
//      }
//      this->set(fireBuf, (palette_entry*)firePalette, true);
//    delay(100);
//  }
 

// //---------------------------------------------------------------------------------------
// // renderStars
// //
// // Renders one frame of the stars animation and displays it immediately
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::renderStars()
// {
// 	// clear buffer
// 	memset(this->currentValues, 0, sizeof(this->currentValues));

// 	// for(StarObject &s : this->stars) s.render(this->currentValues, this->stars);
// }

// //---------------------------------------------------------------------------------------
// // renderHeart
// //
// // Renders one frame of the heart animation and displays it immediately
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::renderHeart()
// {
// 	palette_entry palette[2];
// 	uint8_t heart[] = {
// 		0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0,
// 		1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
// 		0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0,
// 		0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
// 		0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
// 		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
// 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 		1, 1, 1, 1
// 	};
// 	palette[0] = {0, 0, 0};
// 	palette[1] = {(uint8_t)this->heartBrightness, 0, 0};
// 	this->set(heart, palette, true);

// 	switch (this->heartState)
// 	{
// 	case 0:
// 		if (this->heartBrightness >= 255) this->heartState = 1;
// 		else this->heartBrightness += 32;
// 		break;

// 	case 1:
// 		if (this->heartBrightness < 128) this->heartState = 2;
// 		else this->heartBrightness -= 32;
// 		break;

// 	case 2:
// 		if (this->heartBrightness >= 255) this->heartState = 3;
// 		else this->heartBrightness += 32;
// 		break;

// 	case 3:
// 	default:
// 		if (this->heartBrightness <= 0) this->heartState = 0;
// 		else this->heartBrightness -= 4;
// 		break;
// 	}

// 	if (this->heartBrightness > 255) this->heartBrightness = 255;
// 	if (this->heartBrightness < 0) this->heartBrightness = 0;
// }


//   #ifndef PRODUCTION
//   // DEBUG
//   if (Debug.isActive(Debug.DEBUG)){
//     Debug.printf("h=%i, m=%i, s=%i, lastH=%i, lastM=%i\r\n", this->h, this->m, this->s, this->lastH, this->lastM);
//     Debug.println("leavingLetters:");
//     for(xy_t &p : this->leavingLetters)
//     {
//       Debug.printf("  counter=%i, delay=%i, speed=%i, x=%i, y=%i, xTarget=%i, yTarget=%i\r\n",
//           p.counter, p.delay, p.speed, p.x, p.y, p.xTarget, p.yTarget);
//     }
//     Debug.println("arrivingLetters:");
//     for(xy_t &p : this->arrivingLetters)
//     {
//       Debug.printf("  counter=%i, delay=%i, speed=%i, x=%i, y=%i, xTarget=%i, yTarget=%i\r\n",
//           p.counter, p.delay, p.speed, p.x, p.y, p.xTarget, p.yTarget);
//     }
//   }
  
//   #endif

  
  
// }

// //---------------------------------------------------------------------------------------
// // renderFlyingLetters
// //
// // Takes the current arrivingLetters and leavingLetters to render the flying letters
// // animation
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::renderFlyingLetters()
// {
// 	uint8_t buf[NUM_PIXELS];

// 	// load palette colors from configuration
// 	palette_entry palette[] = {
// 		{Config.bg.r, Config.bg.g, Config.bg.b},
// 		{Config.fg.r, Config.fg.g, Config.fg.b},
// 		{Config.s.r,  Config.s.g,  Config.s.b}};

// 	// check if the displayed time has changed
// 	if((this->m/5 != this->lastM/5) || (this->h != this->lastH))
// 	{
// 		// prepare new animation
// 		this->renderTime(buf, this->h, this->m, this->s, this->ms);
// 		this->prepareFlyingLetters(buf);
// 	}

// 	this->lastM = this->m;
// 	this->lastH = this->h;


// 	// create empty buffer filled with seconds color
// 	this->fillBackground(this->s, this->ms, buf);

// 	// minutes 1...4 for the corners
// 	for(int i=0; i<=((this->m%5)-1); i++) buf[10 * 11 + i] = 1;

// 	// leaving letters animation has priority
// 	if(this->leavingLetters.size() > 0)
// 	{
// 		// count actually moved letters to detect end of animation
// 		int movedLetters = 0;

// 		// iterate over all leavingLetters
// 		for(xy_t &p : this->leavingLetters)
// 		{
// 			// draw letter only if inside visible area
// 			if(p.x>=0 && p.y>=0 && p.x<LEDFunctionsClass::width
// 					&& p.y<LEDFunctionsClass::height)
// 				buf[p.x + p.y * LEDFunctionsClass::width] = 1;

// 			// continue with next letter if the current letter already
// 			// reached its target position
// 			if(p.y == p.yTarget && p.x == p.xTarget) continue;
// 			p.counter += p.speed;
// 			movedLetters++;
// 			if(p.counter >= 1000)
// 			{
// 				p.counter -= 1000;
// 				if(p.delay>0)
// 				{
// 					// do not move if animation of current letter is delayed
// 					p.delay--;
// 				}
// 				else
// 				{
// 					if(p.y > p.yTarget) p.y--; else p.y++;
// 				}
// 			}
// 		}
// 		if(movedLetters == 0) this->leavingLetters.clear();
// 	}
// 	else
// 	{
// 		// iterate over all arrivingLetters
// 		for(xy_t &p : this->arrivingLetters)
// 		{
// 			// draw letter only if inside visible area
// 			if(p.x>=0 && p.y>=0 && p.x<LEDFunctionsClass::width
// 					&& p.y<LEDFunctionsClass::height)
// 				buf[p.x + p.y * LEDFunctionsClass::width] = 1;

// 			// continue with next letter if the current letter already
// 			// reached its target position
// 			if(p.y == p.yTarget && p.x == p.xTarget) continue;
// 			p.counter += p.speed;
// 			if(p.counter >= 1000)
// 			{
// 				p.counter -= 1000;
// 				if(p.delay>0)
// 				{
// 					// do not move if animation of current letter is delayed
// 					p.delay--;
// 				}
// 				else
// 				{
// 					if(p.y > p.yTarget) p.y--; else p.y++;
// 				}
// 			}
// 		}
// 	}

// 	// present the current content immediately without fading
// 	this->set(buf, palette, true);
// }

// //---------------------------------------------------------------------------------------
// // renderRed
// //
// // Renders a complete red frame for testing purposes.
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::renderRed()
// {
// 	palette_entry palette[2];
// 	uint8_t heart[] = {
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1
// 	};
// 	palette[0] = {0, 0, 0};
// 	palette[1] = {32, 0, 0};
// 	this->set(heart, palette, true);
// }

// //---------------------------------------------------------------------------------------
// // renderGreen
// //
// // Renders a complete green frame for testing purposes.
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::renderGreen()
// {
// 	palette_entry palette[2];
// 	uint8_t heart[] = {
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1
// 	};
// 	palette[0] = {0, 0, 0};
// 	palette[1] = {0, 32, 0};
// 	this->set(heart, palette, true);
// }

// //---------------------------------------------------------------------------------------
// // renderBlue
// //
// // Renders a complete blue frame for testing purposes.
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::renderBlue()
// {
// 	palette_entry palette[2];
// 	uint8_t heart[] = {
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 		1, 1, 1, 1
// 	};
// 	palette[0] = {0, 0, 0};
// 	palette[1] = {0, 0, 32};
// 	this->set(heart, palette, true);
// }

// //---------------------------------------------------------------------------------------
// // renderUpdate
// //
// // Renders the OTA update screen with progress information depending on
// // Config.updateProgress
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::renderUpdate()
// {
// 	uint8_t update[] = {
// 		0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
// 		0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
// 		0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
// 		0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
// 		0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
// 		0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0,
// 		0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0,
// 		0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
// 		0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
// 		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
// 		1, 1, 1, 1
// 	};
// 	palette_entry p[] = {{0, 0, 0}, {255, 0, 0}, {42, 21, 0}, {255, 85, 0}};
// 	for(int i=0; i<110; i++)
// 	{
// 		if(i<Config.updateProgress)
// 		{
// 			if(update[i] == 0) update[i] = 2;
// 			else update[i] = 3;
// 		}
// 	}
// 	this->set(update, p, true);
// }

// //---------------------------------------------------------------------------------------
// // renderUpdateComplete
// //
// // Renders the OTA update complete screen
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::renderUpdateComplete()
// {
// 	const uint8_t update_ok[] = {
// 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 		0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
// 		0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
// 		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
// 		0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
// 		0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0,
// 		0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
// 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 		1, 1, 1, 1
// 	};
// 	palette_entry p[] = {{0, 21, 0}, {0, 255, 0}};
// 	this->set(update_ok, p, true);
// }

// //---------------------------------------------------------------------------------------
// // renderUpdateError
// //
// // Renders the OTA update error screen
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::renderUpdateError()
// {
// 	const uint8_t update_err[] = {
// 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 		0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
// 		0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
// 		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
// 		0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
// 		0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
// 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 		1, 1, 1, 1
// 	};
// 	palette_entry p[] = {{0, 0, 0}, {255, 0, 0}};
// 	this->set(update_err, p, true);
// }

// //---------------------------------------------------------------------------------------
// // renderWifiManager
// //
// // Renders the WifiManager screen
// //
// // -> --
// // <- --
// //---------------------------------------------------------------------------------------
// void LEDFunctionsClass::renderWifiManager()
// {
// 	const uint8_t wifimanager[] = {
// 		0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
// 		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0,
// 		0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0,
// 		0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
// 		0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
// 		0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
// 		0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
// 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 		0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
// 		0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
// 		1, 1, 1, 1
// 	};
// 	palette_entry p[] = {{0, 0, 0}, {255, 255, 0}};
// 	this->set(wifimanager, p, true);
// }

// #endif
