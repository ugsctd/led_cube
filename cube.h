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
#ifndef _CUBE_H_
#define _CUBE_H_

// #include <IPAddress.h>
#include <stdint.h>

#include <SoftwareSerial.h>
#include <HardwareSerial.h>

#define LAYER_COUNT 8
#define COLUMN_COUNT 64
// structure with configuration data to be stored in EEPROM

enum class AnimationType
{
	Wall,
	Rise,
	Fall,
	flyingLettersVerticalDown,
	explode,
	random,
	matrix,
	heart,
	fire,
	plasma,
	stars,
	red,
	green,
	blue,
	yellowHourglass,
	greenHourglass,
	update,
	updateComplete,
	updateError,
	wifiManager,
	invalid
};

class CubeClass
{
  public:
    // public methods
    CubeClass();
    virtual ~CubeClass();
    void setup(bool altSerial);
    void say(char *what);
    void printLetter(char letter);
    void loopcube();
    void DemoALL_ON();
    void DemoALL_OFF();
    void DemoRise();
    void DemoFall();
    void C();
    void M();
    void Y();
    void R();
    void G();
    void B();

  private:
    void funPrintCube(unsigned char *p);
    unsigned char funGetColumn(unsigned char x, unsigned char y);
    unsigned char pCube[COLUMN_COUNT];
//    bool altTx = true;
    unsigned char letter_T[64]{0x00, 0x60, 0x60, 0x7E, 0x60, 0x60, 0x00}; //T
    unsigned char letter_B[64]{0x1E, 0x22, 0x22, 0x1E, 0x22, 0x22, 0x1E}; //B

    int cube = 1;
    bool go = true;
};

extern CubeClass Cube;

#endif /* _CUBE_H_ */
