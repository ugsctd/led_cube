#include <Arduino.h>
#include <stdint.h>
#include <HardwareSerial.h>
#include "cube.h"
#include "static_pictures.h"
// #include "ntp.h"
#include "config.h"
using namespace std;

// global instance
CubeClass Cube = CubeClass();

bool altTx = true;

// Tries to establish connection to the cube on 38400.
// -> altSerial: true means using Serial1 on D4 (GPIO2)
void CubeClass::setup(bool altSerial)
{
    Serial.println("Cube setup");
    unsigned char i;
    delay(1000);
    altTx = altSerial;
    if (altTx)
        Serial1.begin(38400);
    else
        Serial.begin(38400);

    for (i = 0; i < 10; i++)
    {
        delay(100);
        if (altTx)
            Serial1.write(0xAD);
        else
            Serial.write(0xAD);
    }
    ChangeAnimation((AnimationType)Config.currentAnimation, 'F', Config.currentText, (ColumnColor)Config.currentColor);
}

// Prints the next frame and waits
void CubeClass::printFrame()
{
    unsigned char *p = currentAnimation->printNextFrame();

    if (altTx)
    {
        Serial1.write(0xf2);
        Serial1.write(p, COLUMN_COUNT);
    }
    else
    {
        Serial.write(0xf2);
        Serial.write(p, COLUMN_COUNT);
    }
    delay(Config.delay);
}
//Changes the animation type
void CubeClass::ChangeAnimation(AnimationType t, char param1, String param2, ColumnColor color)
{
    switch (t)
    {
    case AnimationType::Rise:
        currentAnimation = new RiseAnimationClass(param1, param2.toInt(), color);
        break;
    case AnimationType::Fall:
        currentAnimation = new FallAnimationClass(param1, param2.toInt(), color);
        break;
    case AnimationType::Letter:
        currentAnimation = new LetterAnimationClass(param2[0], color);
        break;
    case AnimationType::Blink:
        currentAnimation = new BlinkAnimationClass(param1);
        break;
    case AnimationType::Say:
        currentAnimation = new SayAnimationClass(param2, color);
        break;
    case AnimationType::Wall:
        currentAnimation = new WallAnimationClass(color);
        break;
    case AnimationType::Time:
        currentAnimation = new TimeAnimationClass(color, param2);
        break;
    case AnimationType::Random:
        currentAnimation = new BlinkAnimationClass(10);
        break;
    default:
        break;
    }
    currentAnimation->clear();
}

//  returns the column index in the cube when asked for x,y coordinates
unsigned char AnimationClass::funGetColumn(unsigned char x, unsigned char y)
{
    return (N * y + x);
}
// Function to rotate the matrix 90 degree clockwise
letterArray AnimationClass::rotate90Clockwise(letterArray rows)
{
    letterArray rows3 = {0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            rows3[i] = (((rows[j] & (1 << (N - 1 - i))) >> (N - 1 - i)) << j) | rows3[i];
        }
    }
    return rows3;
}
// Function to rotate the matrix 90 degree anticlockwise

letterArray AnimationClass::rotate90AntiClockwise(letterArray rows)
{
    letterArray rows2 = {0, 0, 0, 0, 0, 0, 0, 0};
    // std::array<unsigned char, N> rows2 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            rows2[i] = (((rows[j] & (1 << i)) >> i) << (N - 1 - j)) | rows2[i];
        }
    }
    return rows2;
}
void AnimationClass::clear()
{
    for (char i = 0; i < COLUMN_COUNT; i++)
        pCube[i] = 0x00;
}

void AnimationClass::fill()
{
    char i;
    for (i = 0; i < COLUMN_COUNT; i++)
        pCube[i] = 0xff;
}

void AnimationClass::zPlus()
{
    char i;
    for (i = 0; i < COLUMN_COUNT; i++)
        pCube[i] >>= 1;
}
void AnimationClass::zMinus()
{
    char i;
    for (i = 0; i < COLUMN_COUNT; i++)
        pCube[i] <<= 1;
}
void AnimationClass::yPlus()
{
    char i;
    for (i = 0; i < COLUMN_COUNT; i++)
        pCube[i] <<= 1;
}
void AnimationClass::yMinus()
{
    char i;
    for (i = 0; i < COLUMN_COUNT; i++)
        pCube[i] >>= 1;
}
void AnimationClass::xPlus()
{
    char i;
    for (i = 0; i < COLUMN_COUNT; i++)
        pCube[i] <<= 1;
}
void AnimationClass::xMinus()
{
    char i;
    for (i = 0; i < COLUMN_COUNT; i++)
        pCube[i] >>= 1;
}
// Constructor, loads default values
RiseAnimationClass::RiseAnimationClass(char density, char length, ColumnColor color)
{
    Serial.println("RiseAnimationClass created");
    density = density < 3 ? 3 : density;
    this->density = density;
    this->length = length;
    this->color = color;
}

// Constructor, loads default values
FallAnimationClass::FallAnimationClass(char density, char length, ColumnColor color)
{
    Serial.println("FallAnimationClass created");
    density = density < 3 ? 3 : density;

    this->density = density;
    this->color = color;
    this->length = length;
}

// Constructor, loads default values
WallAnimationClass::WallAnimationClass(ColumnColor color)
{
    Serial.println("WallAnimationClass created");
    this->color = color;
}

unsigned char *WallAnimationClass::printNextFrame()
{
    unsigned char i;
    clear();
    switch (color)
    {
    case ColumnColor::Red:
        for (i = 0; i < 16; i++)
            pCube[i] = 0xFF;
        for (i = 48; i < 64; i++)
            pCube[i] = 0xFF;
        break;
    case ColumnColor::Green:
        for (i = 16; i < 32; i++)
            pCube[i] = 0xFF;
        break;
    case ColumnColor::Blue:
        for (i = 32; i < 48; i++)
            pCube[i] = 0xFF;
        break;
    case ColumnColor::Cyan:
        for (i = 24; i < 40; i++)
            pCube[i] = 0xFF;
        break;
    case ColumnColor::Magenta:
        for (i = 40; i < 56; i++)
            pCube[i] = 0xFF;
        break;
    case ColumnColor::Yellow:
        for (i = 8; i < 24; i++)
            pCube[i] = 0xFF;
        break;
    case ColumnColor::RedFront:
        for (i = 48; i < 64; i++)
            pCube[i] = 0xFF;
        break;
    case ColumnColor::All:
        fill();
        break;
    case ColumnColor::None:
        break;
    default:
        break;
    }

    return pCube;
}
unsigned char *RiseAnimationClass::printNextFrame()
{
    unsigned char i, x, y;

    for (i = 0; i < COLUMN_COUNT; i++)
    {
        pCube[i] <<= 1;
    }

    for (i = 0; i < random(density); i++)
    {
        x = random(8);

        if (color == ColumnColor::Red)
            y = random(2) + 0;
        else if (color == ColumnColor::Green)
            y = random(2) + 2;
        else if (color == ColumnColor::Blue)
            y = random(2) + 4;
        else if (color == ColumnColor::Cyan)
        {
            y = 3;
            pCube[funGetColumn(x, y + 1)] |= 0x01;
        }
        else if (color == ColumnColor::Magenta)
        {
            y = 5;
            pCube[funGetColumn(x, y + 1)] |= 0x01;
        }
        else if (color == ColumnColor::Yellow)
        {
            y = 1;
            pCube[funGetColumn(x, y + 1)] |= 0x01;
        }
        else
            y = random(8);

        pCube[funGetColumn(x, y)] |= 0x01;
    }
    return pCube;
}
unsigned char *FallAnimationClass::printNextFrame()
{
    unsigned char i, x, y;

    for (i = 0; i < COLUMN_COUNT; i++)
    {
        pCube[i] >>= 1;
    }

    for (i = 0; i < random(density); i++)
    {
        x = random(8);

        if (color == ColumnColor::Red)
            y = random(2) + 0;
        else if (color == ColumnColor::Green)
            y = random(2) + 2;
        else if (color == ColumnColor::Blue)
            y = random(2) + 4;
        else if (color == ColumnColor::Cyan)
        {
            y = 3;
            pCube[funGetColumn(x, y + 1)] |= 0x80;
        }
        else if (color == ColumnColor::Magenta)
        {
            y = 5;
            pCube[funGetColumn(x, y + 1)] |= 0x80;
        }
        else if (color == ColumnColor::Yellow)
        {
            y = 1;
            pCube[funGetColumn(x, y + 1)] |= 0x80;
        }
        else
            y = random(8);

        pCube[funGetColumn(x, y)] |= 0x80;
    }
    return pCube;
}

BlinkAnimationClass::BlinkAnimationClass(int skip)
{
    this->skip = skip;
    Serial.println("BlinkAnimationClass");
}

unsigned char *BlinkAnimationClass::printNextFrame()
{
    // Serial.println("BlinkAnimationClass next frame");
    unsigned char i;
    if (counter % skip == 0)
        for (i = 0; i < 64; i++)
            pCube[i] = 0xff;
    else
        for (i = 0; i < 64; i++)
            pCube[i] = 0x00;
    if (counter == 255)
        counter = 0;
    else
        counter++;
    return pCube;
}

LetterAnimationClass::LetterAnimationClass(char letter, ColumnColor color)
{
    Serial.println(letter);
    this->letter = letter;
    this->color = color;
}

unsigned char *LetterAnimationClass::printNextFrame()
{
    clear();
    letterArray letterRotated = rotate90AntiClockwise(Pictures.alphabet[letter]);

    int row = 7;
    if (color == ColumnColor::Red)
        row = 1;
    if (color == ColumnColor::Green)
        row = 3;
    if (color == ColumnColor::Blue)
        row = 5;
    for (int i = 0; i < 8; i++)
    {
        pCube[i + 8 * row] = letterRotated[i];
    }
    return pCube;
}

SayAnimationClass::SayAnimationClass(String what, ColumnColor color)
{
    Serial.println(what);
    this->what = what;
    this->color = color;
}

void AnimationClass::printImageToColor(letterArray arr, ColumnColor color)
{
    int startRow, endRow = 0;
    if (color == ColumnColor::Red)
    {
        startRow = 0;
        endRow = 1;
    }
    else if (color == ColumnColor::Green)
    {
        startRow = 2;
        endRow = 3;
    }
    else if (color == ColumnColor::Blue)
    {
        startRow = 4;
        endRow = 5;
    }
    else if (color == ColumnColor::Cyan)
    {
        startRow = 3;
        endRow = 4;
    }
    else if (color == ColumnColor::Magenta)
    {
        startRow = 5;
        endRow = 6;
    }
    else if (color == ColumnColor::Yellow)
    {
        startRow = 1;
        endRow = 2;
    }
    else if (color == ColumnColor::RedFront)
    {
        startRow = 6;
        endRow = 7;
    }
    else if (color == ColumnColor::All)
    {
        startRow = 0;
        endRow = 7;
    }

    for (int i = 8 * startRow; i < 8 * (endRow + 1); i++)
        pCube[i] = arr[i % 8];
}
unsigned char *SayAnimationClass::printNextFrame()
{
    clear();
    counter++;
    int letterIndex = counter % 10000;
    if (letterIndex == currentIndex)
        return pCube;
    currentIndex = letterIndex;

    if (letterIndex >= what.length())
    {
        counter = 0;
        letterIndex = counter % 10000;
    }
    letterArray letterRotated = rotate90AntiClockwise(Pictures.alphabet[what[letterIndex]]);

    printImageToColor(letterRotated, color);
    return pCube;
}

// Constructor, loads default values
TimeAnimationClass::TimeAnimationClass(ColumnColor color, String am)
{
    Serial.println("TimeAnimationClass created");
    this->color = color;
    this->am = am;
    // this->say = new SayAnimationClass(sprintf("%d : %d . %d", h, m, s), color);
}

unsigned char *TimeAnimationClass::printNextFrame()
{
    char what[8];
    sprintf(what, "%02i:%02i:%02i", Config.h, Config.m, Config.s);

    Serial.printf("Printing time: %s", what);
    Serial.println("");

    letterArray h1Rotated = rotate90AntiClockwise(Pictures.alphabet[what[0]]);
    letterArray h2Rotated = rotate90AntiClockwise(Pictures.alphabet[what[1]]);
    letterArray m1Rotated = rotate90AntiClockwise(Pictures.alphabet[what[3]]);
    letterArray m2Rotated = rotate90AntiClockwise(Pictures.alphabet[what[4]]);
    letterArray s1Rotated = rotate90AntiClockwise(Pictures.alphabet[what[6]]);
    letterArray s2Rotated = rotate90AntiClockwise(Pictures.alphabet[what[7]]);

    printImageToColor(h1Rotated, ColumnColor::RedFront);
    printImageToColor(h2Rotated, ColumnColor::Blue);
    printImageToColor(m1Rotated, ColumnColor::Green);
    printImageToColor(m2Rotated, ColumnColor::Red);
    return pCube;
}
