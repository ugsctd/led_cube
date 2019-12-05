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

#include <stdint.h>
#include <map>
#include "static_pictures.h"

#include <SoftwareSerial.h>
#include <HardwareSerial.h>

#define LAYER_COUNT 8
#define COLUMN_COUNT 64

using namespace std;

enum class ColumnColor
{
  Red,
  Green,
  Blue,
  Cyan,
  Magenta,
  Yellow,
  All,
  RedFront,
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
  yellowHourglass,
  greenHourglass,
  Update,
  UpdateComplete,
  UpdateError,
  WifiManager,
  Invalid
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
  ColumnColor color;
  void fill();
  void xPlus();
  void xMinus();
  void yPlus();
  void yMinus();
  void zPlus();
  void zMinus();
  void moveAllRight();
  void moveAllLeft();

#define N 8
  void printImageToColor(letterArray arr, ColumnColor color);

  letterArray rotate90Clockwise(letterArray rows);
  letterArray rotate90AntiClockwise(letterArray rows);
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
};
//Randomly generated particles start at the top to dissappear at the bottom
class FallAnimationClass : public AnimationClass
{
public:
  FallAnimationClass(char density, char length, ColumnColor color);
  unsigned char *printNextFrame();

private:
  char length;
  char density;
};

// Prints a letter/digit in a given color
class LetterAnimationClass : public AnimationClass
{
public:
  LetterAnimationClass(char letter, ColumnColor color);
  unsigned char *printNextFrame();

private:
  char letter;
};

// Prints a letter/digit in a given color
class SayAnimationClass : public AnimationClass
{
public:
  SayAnimationClass(String what, ColumnColor color);
  unsigned char *printNextFrame();

private:
  String what;
  int counter = 0;
  int currentIndex = 0;
};
class TimeAnimationClass : public AnimationClass
{
public:
  TimeAnimationClass(ColumnColor color, String am);
  unsigned char *printNextFrame();

private:
  int hour;
  int minute;
  int second;
  String am;
};

class RandomAnimationClass : public AnimationClass
{
public:
  RandomAnimationClass();
  unsigned char *printNextFrame();

private:
  char letter;
  SayAnimationClass say;
};
//Entry class for all your LED cube needs
class CubeClass
{
public:
  void setup(bool altSerial);
  void printFrame();
  void ChangeAnimation(AnimationType t, char param1, String param2, ColumnColor param3);
  AnimationClass *currentAnimation;

private:
  unsigned char pCube[COLUMN_COUNT];

  int cube = 1;
  bool go = true;
};

extern CubeClass Cube;

extern RiseAnimationClass RiseAnimation;
extern FallAnimationClass FallAnimation;
#endif /* _CUBE_H_ */
