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
#include <map>

#include <SoftwareSerial.h>
#include <HardwareSerial.h>

#define LAYER_COUNT 8
#define COLUMN_COUNT 64
// structure with configuration data to be stored in EEPROM
using namespace std;

// std::map<char, letterArray> alphabet;

enum class ColumnColor
{
  Red,
  Green,
  Blue,
  Cyan,
  Magenta,
  Yellow,
  All,
  None
};

enum class AnimationType
{
  Rise,
  Fall,
  Letter,
  Say,
  Wall,
  Blink,
  Time,
  Random,
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
  void clear();

protected:
  unsigned char funGetColumn(unsigned char x, unsigned char y);
  unsigned char pCube[COLUMN_COUNT];
  void fill();
  void xPlus();
  void xMinus();
  void yPlus();
  void yMinus();
  void zPlus();
  void zMinus();
  void moveAllRight();
  void moveAllLeft();
  
#define N 4
  void rotate90Clockwise(int a[N][N]);
};

//Randomly generated particles start at the bottom to dissappear at the top
class RiseAnimationClass : public AnimationClass
{
public:
  RiseAnimationClass(char density, char length, ColumnColor color);
  unsigned char *printNextFrame();

private:
  char length;
  char density;
  ColumnColor color;
};

//Flashes the whole cube
class BlinkAnimationClass : public AnimationClass
{
public:
  BlinkAnimationClass(int skip);
  unsigned char *printNextFrame();

private:
  int skip;
  char counter = 0;
};

//Lights up a whole plane of selected color
class WallAnimationClass : public AnimationClass
{
public:
  WallAnimationClass(ColumnColor color);
  unsigned char *printNextFrame();

private:
  ColumnColor color;
};
//Randomly generated particles start at the top to dissappear at the bottom
class FallAnimationClass : public AnimationClass
{
public:
  FallAnimationClass(char density, char length, ColumnColor color);
  unsigned char *printNextFrame();

private:
  char length;
  ColumnColor color;
  char density;
};

// Prints a letter/digit in a given color
class LetterAnimationClass : public AnimationClass
{
public:
  LetterAnimationClass(char letter, ColumnColor color);
  unsigned char *printNextFrame();

private:
  ColumnColor color;
  char letter;
};

class TimeAnimationClass : public AnimationClass
{
public:
  // TimeAnimationClass(char h, char m, char s);
  TimeAnimationClass(String time);
  unsigned char *printNextFrame();

private:
  char h;
  char m;
  char s;
};

class RandomAnimationClass : public AnimationClass
{
public:
  RandomAnimationClass();
  unsigned char *printNextFrame();

private:
  ColumnColor color;
  char letter;
};
//Entry class for all your LED cube needs
class CubeClass
{
public:
  void setup(bool altSerial);
  void printFrame();
  void ChangeAnimation(AnimationType t, char param1, String param2, ColumnColor param3);
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
