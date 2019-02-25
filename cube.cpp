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
    currentAnimation = new RiseAnimationClass(3, 1);
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
void CubeClass::ChangeAnimation(AnimationType t, int param1, String param2)
{
    this->type = t;
    switch (t)
    {
    case AnimationType::Rise:
        currentAnimation = new RiseAnimationClass(param1, param2.toInt());
        break;
    case AnimationType::Fall:
        currentAnimation = new FallAnimationClass(param1, param2.toInt());
        break;
    case AnimationType::Letter:
        currentAnimation = new LetterAnimationClass(param2[0], ColumnColor::Red);
        break;
    case AnimationType::Blink:
        currentAnimation = new BlinkAnimationClass();
        break;
    case AnimationType::Random:
        currentAnimation = new BlinkAnimationClass();
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

// Constructor, loads default values
RiseAnimationClass::RiseAnimationClass(char density, char length)
{
    Serial.println("RiseAnimationClass created");
    this->density = density;
    this->length = length;
}

// Constructor, loads default values
FallAnimationClass::FallAnimationClass(char density, char length)
{
    Serial.println("FallAnimationClass created");
    this->density = density;
    this->length = length;
}
// Constructor, loads default values
LetterAnimationClass::LetterAnimationClass(char letter, ColumnColor color)
{
    this->letter = letter;
    this->color = color;
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
BlinkAnimationClass::BlinkAnimationClass()
{
    Serial.println("BlinkAnimationClass");
}

unsigned char *BlinkAnimationClass::printNextFrame()
{
    // Serial.println("BlinkAnimationClass next frame");
    unsigned char i;
    if (counter % 20 == 0)
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
