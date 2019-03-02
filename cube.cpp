#include <Arduino.h>
#include <stdint.h>
#include <HardwareSerial.h>
#include "cube.h"
#include "config.h"

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
    currentAnimation = new RiseAnimationClass(3, 1, ColumnColor::Red);
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
    this->type = t;
    switch (t)
    {
    case AnimationType::Rise:
        currentAnimation = new RiseAnimationClass(param1, param2.toInt(), color);
        break;
    case AnimationType::Fall:
        currentAnimation = new FallAnimationClass(param1, param2.toInt());
        break;
    case AnimationType::Letter:
        currentAnimation = new LetterAnimationClass(param2[0], color);
        break;
    case AnimationType::Blink:
        currentAnimation = new BlinkAnimationClass(param1);
        break;
    case AnimationType::Wall:
        currentAnimation = new WallAnimationClass(color);
        break;
    case AnimationType::Random:
        currentAnimation = new BlinkAnimationClass(10);
        break;
    default:
        break;
    }
}

//  returns the column index in the cube when asked for x,y coordinates
unsigned char AnimationClass::funGetColumn(unsigned char x, unsigned char y)
{
    return (8 * y + x);
}

void AnimationClass::clear()
{
    char i;
    for (i = 0; i < COLUMN_COUNT; i++)
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
    // density = density < 2 ? 2 : density;
    this->density = density;
    this->length = length;
    this->color = color;
}

// Constructor, loads default values
FallAnimationClass::FallAnimationClass(char density, char length)
{
    Serial.println("FallAnimationClass created");
    density = density < 2 ? 2 : density;
    this->density = density;
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
    case ColumnColor::All:
        for (i = 0; i < 64; i++)
            pCube[i] = 0xFF;
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
        y = random(8);
        pCube[funGetColumn(x, y)] |= 0x80;
    }
    return pCube;
}
// Constructor, loads default values
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
// Constructor, loads default values
LetterAnimationClass::LetterAnimationClass(char letter, ColumnColor color)
{
    Serial.println(letter);
    this->letter = letter;
    this->color = color;
}
unsigned char *LetterAnimationClass::printNextFrame()
{
    switch (letter)
    {
    case '1':
        return letter_1;
        break;
    case '2':
        return letter_2;
        break;
    case '3':
        return letter_3;
        break;
    case '4':
        return letter_4;
        break;
    case '5':
        return letter_5;
        break;
    case '6':
        return letter_6;
        break;
    case '7':
        return letter_7;
        break;
    case '8':
        return letter_8;
        break;
    case '9':
        return letter_9;
        break;
    case '0':
        return letter_0;
        break;
    case 't':
    case 'T':
        return letter_T;
        break;
    case 'b':
    case 'B':
        return letter_B;
        break;
    case 'x':
    case 'X':
        return letter_X;
        break;
    default:
        return letter_X;
        break;
    }
}
