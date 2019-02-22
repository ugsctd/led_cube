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
#ifndef _CUBE_EXAMPLE_H_
#define _CUBE_EXAMPLE_H_

// #include <IPAddress.h>
#include <stdint.h>

// #define NUM_PIXELS 114
// #define HOURGLASS_ANIMATION_FRAMES 8

#define LAYER_COUNT 8
#define COLUMN_COUNT 64
// structure with configuration data to be stored in EEPROM

class CubeClass
{
  public:
    // public methods
    CubeClass();
    virtual ~CubeClass();
    void setup(bool altSerial);
    void say(String what);
    void printLetter(char letter);
    void loopcube();
    void DemoALL_ON(byte *pCube);
    void DemoALL_OFF(byte *pCube);
    void DemoRise(byte *pCube);
    void DemoFall(byte *pCube);

  private:
    void funPrintCube(byte *p);
    void funGetColumn(byte x, byte y);
    unsigned char pCube[COLUMN_COUNT];
    bool altTx = true;
    unsigned char letter_T[64]{0x00, 0x60, 0x60, 0x7E, 0x60, 0x60, 0x00}; //T
    unsigned char letter_B[64]{0x1E, 0x22, 0x22, 0x1E, 0x22, 0x22, 0x1E}; //B

    int cube = 1;
    bool go = true;
};

extern CubeClass Cube;

#endif /* _CUBE_EXAMPLE_H_ */
