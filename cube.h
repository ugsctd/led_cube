// 8x8x8 LED Cube
// Copyright (C) 2019 Michał Korzunowicz, https://github.com/mkorzunowicz
//
//  See cube.h for description.
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

enum class ColumnColor
{
  Red,
  Green,
  Blue,
  Cyan,
  Magenta,
  Yellow
};

enum class AnimationType
{
  Wall,
  Rise,
  Fall,
  Letter,
  Say,
  Random,
  Blink,
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

//Abstract class for animations
class AnimationClass
{
public:
  virtual unsigned char *printNextFrame() = 0;

protected:
  unsigned char funGetColumn(unsigned char x, unsigned char y);
  unsigned char pCube[COLUMN_COUNT];
};

//Randomly generated particles start at the bottom to dissappear at the top
class RiseAnimationClass : public AnimationClass
{
public:
  RiseAnimationClass(char density, char length);
  unsigned char *printNextFrame();

private:
  char length;
  char density;
};

//Flashes the whole cube
class BlinkAnimationClass : public AnimationClass
{
public:
  BlinkAnimationClass();
  unsigned char *printNextFrame();

private:
  char counter = 0;
};
//Randomly generated particles start at the top to dissappear at the bottom
class FallAnimationClass : public AnimationClass
{
public:
  FallAnimationClass(char density, char length);
  unsigned char *printNextFrame();

private:
  char length;
  char density;
};

class LetterAnimationClass : public AnimationClass
{
public:
  LetterAnimationClass(char letter, ColumnColor color);
  unsigned char *printNextFrame();

private:
  ColumnColor color;
  char letter;
  unsigned char letter_1[64]{0x10, 0x20, 0x40, 0x81, 0xff, 0x01, 0x01, 0x00};
  unsigned char letter_2[64]{0x21, 0x43, 0x83, 0x85, 0x89, 0x49, 0x31, 0x01};
  unsigned char letter_3[64]{0x00, 0x00, 0x00, 0x42, 0x81, 0x99, 0x66, 0x00};
  unsigned char letter_4[64]{0x00, 0x00, 0x18, 0x28, 0x48, 0xfe, 0x08, 0x00};
  unsigned char letter_5[64]{0x00, 0x00, 0xf4, 0x92, 0x92, 0x8c, 0x00, 0x00};
  unsigned char letter_6[64]{0x00, 0x00, 0x7c, 0x92, 0x92, 0x4c, 0x00, 0x00};

  unsigned char letter_T[64]{0xc0, 0xc0, 0xc0, 0xff, 0xff, 0xc0, 0xc0, 0xc0};
  unsigned char letter_B[64]{0x1E, 0x22, 0x22, 0x1E, 0x22, 0x22, 0x1E};
  unsigned char letter_X[64]{0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81};
};

class RandomAnimationClass : public AnimationClass
{
public:
  RandomAnimationClass();
  unsigned char *printNextFrame();

private:
  ColumnColor color;
  char letter;

  unsigned char letter_T[64]{0x00, 0x60, 0x60, 0x7E, 0x60, 0x60, 0x00}; //T
  unsigned char letter_B[64]{0x1E, 0x22, 0x22, 0x1E, 0x22, 0x22, 0x1E}; //B
};
//Entry class for all your LED cube needs
class CubeClass
{
public:
  void setup(bool altSerial);
  void printFrame();
  void ChangeAnimation(AnimationType t, int param1, String param2);
  AnimationType type = AnimationType::Rise;

private:
  unsigned char pCube[COLUMN_COUNT];
  AnimationClass *currentAnimation;

  int cube = 1;
  bool go = true;
};

extern CubeClass Cube;

extern RiseAnimationClass RiseAnimation;
extern FallAnimationClass FallAnimation;
#endif /* _CUBE_H_ */
