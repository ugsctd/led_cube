// #include <stdint.h>
#include <cube_example.h>

// #define LAYER_COUNT 8
// #define COLUMN_COUNT 64
// byte pCube[COLUMN_COUNT];

// byte letter_T[64]{0x00, 0x60, 0x60, 0x7E, 0x60, 0x60, 0x00}; //T
// byte letter_B[64]{0x1E, 0x22, 0x22, 0x1E, 0x22, 0x22, 0x1E}; //B

// int cube = 1;
// bool go = true;
//---------------------------------------------------------------------------------------
// global instance
//---------------------------------------------------------------------------------------
CubeClass Cube = CubeClass();
//---------------------------------------------------------------------------------------
// CubeClass
//
// Constructor, loads default values
//
// -> --
// <- --
//---------------------------------------------------------------------------------------
CubeClass::CubeClass()
{
    // this->reset();
}

//---------------------------------------------------------------------------------------
// ~CubeClass
//
// destructor
//
// -> --
// <- --
//---------------------------------------------------------------------------------------
CubeClass::~CubeClass()
{
}

//---------------------------------------------------------------------------------------
// setup
//
// Tries to establish connection to the cube on 38400.
//
//
// -> altSerial: true means using Serial1 on D4 (GPIO2)
// <- --
//---------------------------------------------------------------------------------------
void CubeClass::setup(bool altSerial)
{
    byte i;
    delay(1000);
    altTx = altSerial;
    if (altTx) //??
        Serial1.begin(38400);
    else
        Serial.begin(38400);

    for (i = 0; i < 10; i++)
    {
        delay(100);
        if (altTx) //??
            Serial1.write(0xAD);
        else
            Serial.write(0xAD);
    }
}

//---------------------------------------------------------------------------------------
// say
//
// Renders sentence
//
// -> what: what to say
// <- --
//---------------------------------------------------------------------------------------
void CubeClass::say(String what)
{
    // printLetter(what);
}

//---------------------------------------------------------------------------------------
// printLetter
//
// Renders a letter.
//
// -> letter: letter to print. i.e. 'T'
// <- --
//---------------------------------------------------------------------------------------
void CubeClass::printLetter(char letter)
{
    byte j;
    j = 10;
    while (j--)
    {
        switch (letter)
        {
        case 't':
            funPrintCube(letter_T);
            break;
        case 'b':
            funPrintCube(letter_B);
            break;
        default:
            break;
        }
    }
}

//---------------------------------------------------------------------------------------
// loopcube
//
// Runs following sequence: None, Rise, All
//
//---------------------------------------------------------------------------------------
void CubeClass::loopcube()
{
    DemoALL_OFF(pCube);
    DemoRise(pCube);
    DemoALL_ON(pCube);
}

//---------------------------------------------------------------------------------------
// DemoALL_ON
//
// Lits all LEDs up
//
// -> altSerial: true means using Serial1 on D4 (GPIO2)
// <- --
//---------------------------------------------------------------------------------------
void CubeClass::DemoALL_ON(byte *pCube)
{
    byte i, j;
    j = 10;
    while (j--)
    {
        for (i = 0; i < 64; i++)
        {
            pCube[i] = 0xff;
        }
        funPrintCube(pCube);
    }
}

//---------------------------------------------------------------------------------------
// DemoALL_OFF
//
//Turns every LED off.
//
// -> pCube: cube to print
// <- --
//---------------------------------------------------------------------------------------
void CubeClass::DemoALL_OFF(byte *pCube)
{
    byte i, j;
    j = 10;
    while (j--)
    {
        for (i = 0; i < 64; i++)
        {
            pCube[i] = 0x00;
        }
        funPrintCube(pCube);
    }
}

//---------------------------------------------------------------------------------------
// DemoRise
//
//
// -> pCube: cube to print
// <- --
//---------------------------------------------------------------------------------------
void CubeClass::DemoRise(byte *pCube)
{
    byte i, x, y, j;

    j = 20;
    while (j--)
    {
        for (i = 0; i < COLUMN_COUNT; i++)
        {
            pCube[i] <<= 1;
        }

        for (i = 0; i < random(4); i++)
        {
            x = random(8);
            y = random(8);
            pCube[funGetColumn(x, y)] |= 0x01;
        }

        funPrintCube(pCube);
    }
}

//---------------------------------------------------------------------------------------
// DemoFall
//
//
// -> pCube: cube to print
// <- --
//---------------------------------------------------------------------------------------
void CubeClass::DemoFall(byte *pCube)
{
    byte i, x, y, j;

    j = 20;
    while (j--)
    {
        for (i = 0; i < COLUMN_COUNT; i++)
        {
            pCube[i] <<= 1;
        }

        for (i = 0; i < random(4); i++)
        {
            x = random(8);
            y = random(8);
            pCube[funGetColumn(x, y)] |= 0x01;
        }

        funPrintCube(pCube);
    }
}

//---------------------------------------------------------------------------------------
// funPrintCube
//
// Tries to establish connection to the cube on 38400.
//
// -> *p: byte array to print
// <- --
//---------------------------------------------------------------------------------------
void funPrintCube(byte *p)
{
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

    delay(40);
}

//---------------------------------------------------------------------------------------
// funGetColumn
//
//
//
// -> x: dunno
// -> y:
// <- --
//---------------------------------------------------------------------------------------
void funGetColumn(byte x, byte y)
{
    return (8 * y + x);
}