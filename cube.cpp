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
    unsigned char* p = currentAnimation->printNextFrame();

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
void CubeClass::ChangeAnimation(AnimationType t)
{
    this->type = t;
    switch (t)
    {
    case AnimationType::Rise:
        currentAnimation = new RiseAnimationClass(3, 1);
        break;
    case AnimationType::Fall:
        currentAnimation = new FallAnimationClass(3, 1);
        break;
    case AnimationType::Letter:
        currentAnimation = new LetterAnimationClass('t', ColumnColor::Red);
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

unsigned char* RiseAnimationClass::printNextFrame()
{
    // Serial.println("RiseAnimationClass next frame");

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

unsigned char* FallAnimationClass::printNextFrame()
{
    // Serial.println("FallAnimationClass next frame");

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
BlinkAnimationClass::BlinkAnimationClass(char frequency)
{
    this->frequency = frequency;
    Serial.println("BlinkAnimationClass");
}

unsigned char* BlinkAnimationClass::printNextFrame()
{
    // Serial.println("BlinkAnimationClass next frame");
    unsigned char i;
    if (counter % frequency == 0)
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
unsigned char* LetterAnimationClass::printNextFrame()
{
    switch (letter)
    {
    case 't':
        //    funPrintCube(letter_T);
        break;
    case 'b':
        //     funPrintCube(letter_B);
        break;
    default:
        break;
    }
    return pCube;
}
