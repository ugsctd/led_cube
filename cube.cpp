#include <Arduino.h>
#include <stdint.h>
#include <HardwareSerial.h>
#include "cube.h"
#include "config.h"
using namespace std;

// global instance
CubeClass Cube = CubeClass();

typedef std::array<unsigned char, 64> letterArray;
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
    currentAnimation = new RiseAnimationClass(3, 1, ColumnColor::All);
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
        currentAnimation = new FallAnimationClass(param1, param2.toInt(), color);
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
    case AnimationType::Time:
        currentAnimation = new TimeAnimationClass(param2);
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
    return (8 * y + x);
}
// Function to rotate the matrix 90 degree clockwise
void AnimationClass::rotate90Clockwise(int a[N][N])
{

    // Traverse each cycle
    for (int i = 0; i < N / 2; i++)
    {
        for (int j = i; j < N - i - 1; j++)
        {

            // Swap elements of each cycle
            // in clockwise direction
            int temp = a[i][j];
            a[i][j] = a[N - 1 - j][i];
            a[N - 1 - j][i] = a[N - 1 - i][N - 1 - j];
            a[N - 1 - i][N - 1 - j] = a[j][N - 1 - i];
            a[j][N - 1 - i] = temp;
        }
    }
}
void AnimationClass::clear()
{
    // char i;
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
    // density = density < 2 ? 2 : density;
    this->density = density;
    this->length = length;
    this->color = color;
}

// Constructor, loads default values
FallAnimationClass::FallAnimationClass(char density, char length, ColumnColor color)
{
    Serial.println("FallAnimationClass created");
    // density = density < 2 ? 2 : density;
    this->density = density;
    this->color = color;
    this->length = length;
}

// Constructor, loads default values
TimeAnimationClass::TimeAnimationClass(String time)
{
    Serial.println("TimeAnimationClass created");
    // this->color = color;
}

unsigned char *TimeAnimationClass::printNextFrame()
{
    return pCube;
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

std::map<char, letterArray> create_alphabet()
{
    std::map<char, letterArray> alphabet;
    alphabet['1'] = {
        B00010000,
        B00110000,
        B00010000,
        B00010000,
        B00010000,
        B00010000,
        B00010000,
        B00111000};

    alphabet['2'] = {
        B00111000,
        B01000100,
        B00000100,
        B00000100,
        B00001000,
        B00010000,
        B00100000,
        B01111100};
    alphabet['3'] = {
        B00111000,
        B01000100,
        B00000100,
        B00011000,
        B00000100,
        B00000100,
        B01000100,
        B00111000};
    alphabet['4'] = {
        B00000100,
        B00001100,
        B00010100,
        B00100100,
        B01000100,
        B01111100,
        B00000100,
        B00000100};
    alphabet['5'] = {
        B01111100,
        B01000000,
        B01000000,
        B01111000,
        B00000100,
        B00000100,
        B01000100,
        B00111000};
    alphabet['6'] = {
        B00111000,
        B01000100,
        B01000000,
        B01111000,
        B01000100,
        B01000100,
        B01000100,
        B00111000};
    alphabet['7'] = {
        B01111100,
        B00000100,
        B00000100,
        B00001000,
        B00010000,
        B00100000,
        B00100000,
        B00100000};
    alphabet['8'] = {
        B00111000,
        B01000100,
        B01000100,
        B00111000,
        B01000100,
        B01000100,
        B01000100,
        B00111000};
    alphabet['9'] = {
        B00111000,
        B01000100,
        B01000100,
        B01000100,
        B00111100,
        B00000100,
        B01000100,
        B00111000};
    alphabet['0'] = {
        B00111000,
        B01000100,
        B01000100,
        B01000100,
        B01000100,
        B01000100,
        B01000100,
        B00111000};

    alphabet['A'] = {
        B00110000,
        B01111000,
        B11001100,
        B11001100,
        B11111100,
        B11001100,
        B11001100,
        B00000000};
    alphabet['B'] = {
        B11111100,
        B01100110,
        B01100110,
        B01111100,
        B01100110,
        B01100110,
        B11111100,
        B00000000};
    alphabet['C'] = {
        B00111100,
        B01100110,
        B11000000,
        B11000000,
        B11000000,
        B01100110,
        B00111100,
        B00000000};
    alphabet['D'] = {
        B11111000,
        B01101100,
        B01100110,
        B01100110,
        B01100110,
        B01101100,
        B11111000,
        B00000000};
    alphabet['E'] = {
        B11111110,
        B01100010,
        B01101000,
        B01111000,
        B01101000,
        B01100010,
        B11111110,
        B00000000};
    alphabet['F'] = {
        B11111110,
        B01100010,
        B01101000,
        B01111000,
        B01101000,
        B01100000,
        B11110000,
        B00000000};
    alphabet['G'] = {
        B00111100,
        B01100110,
        B11000000,
        B11000000,
        B11001110,
        B01100110,
        B00111110,
        B00000000};
    alphabet['H'] = {
        B11001100,
        B11001100,
        B11001100,
        B11111100,
        B11001100,
        B11001100,
        B11001100,
        B00000000};
    alphabet['I'] = {
        B01111000,
        B00110000,
        B00110000,
        B00110000,
        B00110000,
        B00110000,
        B01111000,
        B00000000};
    alphabet['J'] = {
        B00011110,
        B00001100,
        B00001100,
        B00001100,
        B11001100,
        B11001100,
        B01111000,
        B00000000};
    alphabet['K'] = {
        B11100110,
        B01100110,
        B01101100,
        B01111000,
        B01101100,
        B01100110,
        B11100110,
        B00000000};
    alphabet['L'] = {
        B11110000,
        B01100000,
        B01100000,
        B01100000,
        B01100010,
        B01100110,
        B11111110,
        B00000000};
    alphabet['M'] = {
        B11000110,
        B11101110,
        B11111110,
        B11111110,
        B11010110,
        B11000110,
        B11000110,
        B00000000};
    alphabet['N'] = {
        B11000110,
        B11100110,
        B11110110,
        B11011110,
        B11001110,
        B11000110,
        B11000110,
        B00000000};
    alphabet['O'] = {
        B00111000,
        B01101100,
        B11000110,
        B11000110,
        B11000110,
        B01101100,
        B00111000,
        B00000000};
    alphabet['P'] = {
        B11111100,
        B01100110,
        B01100110,
        B01111100,
        B01100000,
        B01100000,
        B11110000,
        B00000000};
    alphabet['Q'] = {
        B01111000,
        B11001100,
        B11001100,
        B11001100,
        B11011100,
        B01111000,
        B00011100,
        B00000000};
    alphabet['R'] = {
        B11111100,
        B01100110,
        B01100110,
        B01111100,
        B01101100,
        B01100110,
        B11100110,
        B00000000};
    alphabet['S'] = {
        B01111000,
        B11001100,
        B11100000,
        B01110000,
        B00011100,
        B11001100,
        B01111000,
        B00000000};
    alphabet['T'] = {
        B11111100,
        B10110100,
        B00110000,
        B00110000,
        B00110000,
        B00110000,
        B01111000,
        B00000000};
    alphabet['U'] = {
        B11001100,
        B11001100,
        B11001100,
        B11001100,
        B11001100,
        B11001100,
        B11111100,
        B00000000};
    alphabet['V'] = {
        B11001100,
        B11001100,
        B11001100,
        B11001100,
        B11001100,
        B01111000,
        B00110000,
        B00000000};
    alphabet['W'] = {
        B11000110,
        B11000110,
        B11000110,
        B11010110,
        B11111110,
        B11101110,
        B11000110,
        B00000000};
    alphabet['X'] = {
        B11000110,
        B11000110,
        B01101100,
        B00111000,
        B00111000,
        B01101100,
        B11000110,
        B00000000};
    alphabet['Y'] = {
        B11001100,
        B11001100,
        B11001100,
        B01111000,
        B00110000,
        B00110000,
        B01111000,
        B00000000};
    alphabet['Z'] = {
        B11111110,
        B11000110,
        B10001100,
        B00011000,
        B00110010,
        B01100110,
        B11111110,
        B00000000};
    alphabet[' '] = {
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000};
    alphabet['a'] = {
        B00000000,
        B00000000,
        B01111000,
        B00001100,
        B01111100,
        B11001100,
        B01110110,
        B00000000};
    alphabet['b'] = {
        B11100000,
        B01100000,
        B01100000,
        B01111100,
        B01100110,
        B01100110,
        B11011100,
        B00000000};
    alphabet['c'] = {
        B00000000,
        B00000000,
        B01111000,
        B11001100,
        B11000000,
        B11001100,
        B01111000,
        B00000000};
    alphabet['d'] = {
        B00011100,
        B00001100,
        B00001100,
        B01111100,
        B11001100,
        B11001100,
        B01110110,
        B00000000};
    alphabet['e'] = {
        B00000000,
        B00000000,
        B01111000,
        B11001100,
        B11111100,
        B11000000,
        B01111000,
        B00000000};
    alphabet['f'] = {
        B00111000,
        B01101100,
        B01100000,
        B11110000,
        B01100000,
        B01100000,
        B11110000,
        B00000000};
    alphabet['g'] = {
        B00000000,
        B00000000,
        B01110110,
        B11001100,
        B11001100,
        B01111100,
        B00001100,
        B11111000};
    alphabet['h'] = {
        B11100000,
        B01100000,
        B01101100,
        B01110110,
        B01100110,
        B01100110,
        B11100110,
        B00000000};
    alphabet['i'] = {
        B00110000,
        B00000000,
        B01110000,
        B00110000,
        B00110000,
        B00110000,
        B01111000,
        B00000000};
    alphabet['j'] = {
        B00001100,
        B00000000,
        B00001100,
        B00001100,
        B00001100,
        B11001100,
        B11001100,
        B01111000};
    alphabet['k'] = {
        B11100000,
        B01100000,
        B01100110,
        B01101100,
        B01111000,
        B01101100,
        B11100110,
        B00000000};
    alphabet['l'] = {
        B01110000,
        B00110000,
        B00110000,
        B00110000,
        B00110000,
        B00110000,
        B01111000,
        B00000000};
    alphabet['m'] = {
        B00000000,
        B00000000,
        B11001100,
        B11111110,
        B11111110,
        B11010110,
        B11000110,
        B00000000};
    alphabet['n'] = {
        B00000000,
        B00000000,
        B11111000,
        B11001100,
        B11001100,
        B11001100,
        B11001100,
        B00000000};
    alphabet['o'] = {
        B00000000,
        B00000000,
        B01111000,
        B11001100,
        B11001100,
        B11001100,
        B01111000,
        B00000000};
    alphabet['p'] = {
        B00000000,
        B00000000,
        B11011100,
        B01100110,
        B01100110,
        B01111100,
        B01100000,
        B11110000};
    alphabet['q'] = {
        B00000000,
        B00000000,
        B01110110,
        B11001100,
        B11001100,
        B01111100,
        B00001100,
        B00011110};
    alphabet['r'] = {
        B00000000,
        B00000000,
        B11011100,
        B01110110,
        B01100110,
        B01100000,
        B11110000,
        B00000000};
    alphabet['s'] = {
        B00000000,
        B00000000,
        B01111100,
        B11000000,
        B01111000,
        B00001100,
        B11111000,
        B00000000};
    alphabet['t'] = {
        B00010000,
        B00110000,
        B01111100,
        B00110000,
        B00110000,
        B00110100,
        B00011000,
        B00000000};
    alphabet['u'] = {
        B00000000,
        B00000000,
        B11001100,
        B11001100,
        B11001100,
        B11001100,
        B01110110,
        B00000000};
    alphabet['v'] = {
        B00000000,
        B00000000,
        B11001100,
        B11001100,
        B11001100,
        B01111000,
        B00110000,
        B00000000};
    alphabet['w'] = {
        B00000000,
        B00000000,
        B11000110,
        B11010110,
        B11111110,
        B11111110,
        B01101100,
        B00000000};
    alphabet['x'] = {
        B00000000,
        B00000000,
        B11000110,
        B01101100,
        B00111000,
        B01101100,
        B11000110,
        B00000000};
    alphabet['y'] = {
        B00000000,
        B00000000,
        B11001100,
        B11001100,
        B11001100,
        B01111100,
        B00001100,
        B11111000};
    alphabet['z'] = {
        B00000000,
        B00000000,
        B11111100,
        B10011000,
        B00110000,
        B01100100,
        B11111100,
        B00000000};

    alphabet['+'] = {
        B00000000,
        B00110000,
        B00110000,
        B11111100,
        B00110000,
        B00110000,
        B00000000,
        B00000000};
    alphabet['-'] = {
        B00000000,
        B00000000,
        B00000000,
        B11111100,
        B00000000,
        B00000000,
        B00000000,
        B00000000};
    alphabet['*'] = {
        B00000000,
        B01100110,
        B00111100,
        B11111111,
        B00111100,
        B01100110,
        B00000000,
        B00000000};
    alphabet['/'] = {
        B00000110,
        B00001100,
        B00011000,
        B00110000,
        B01100000,
        B11000000,
        B10000000,
        B00000000};
    alphabet['%'] = {
        B00000000,
        B11000110,
        B11001100,
        B00011000,
        B00110000,
        B01100110,
        B11000110,
        B00000000};
    alphabet['='] = {
        B00000000,
        B00000000,
        B11111100,
        B00000000,
        B00000000,
        B11111100,
        B00000000,
        B00000000};
    alphabet['~'] = {
        B01110110,
        B11011100,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000};
    alphabet['^'] = {
        B00010000,
        B00111000,
        B01101100,
        B11000110,
        B00000000,
        B00000000,
        B00000000,
        B00000000};
    alphabet['<'] = {
        B00011000,
        B00110000,
        B01100000,
        B11000000,
        B01100000,
        B00110000,
        B00011000,
        B00000000};
    alphabet['>'] = {
        B01100000,
        B00110000,
        B00011000,
        B00001100,
        B00011000,
        B00110000,
        B01100000,
        B00000000};
    alphabet['('] = {
        B00011000,
        B00110000,
        B01100000,
        B01100000,
        B01100000,
        B00110000,
        B00011000,
        B00000000};
    alphabet[')'] = {
        B01100000,
        B00110000,
        B00011000,
        B00011000,
        B00011000,
        B00110000,
        B01100000,
        B00000000};
    alphabet['['] = {
        B01111000,
        B01100000,
        B01100000,
        B01100000,
        B01100000,
        B01100000,
        B01111000,
        B00000000};
    alphabet[']'] = {
        B01111000,
        B00011000,
        B00011000,
        B00011000,
        B00011000,
        B00011000,
        B01111000,
        B00000000};
    alphabet['{'] = {
        B00011100,
        B00110000,
        B00110000,
        B11100000,
        B00110000,
        B00110000,
        B00011100,
        B00000000};
    alphabet['}'] = {
        B11100000,
        B00110000,
        B00110000,
        B00011100,
        B00110000,
        B00110000,
        B11100000,
        B00000000};
    alphabet['.'] = {
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00110000,
        B00110000,
        B00000000};
    alphabet[':'] = {
        B00000000,
        B00110000,
        B00110000,
        B00000000,
        B00000000,
        B00110000,
        B00110000,
        B00000000};
    alphabet[';'] = {
        B00000000,
        B00110000,
        B00110000,
        B00000000,
        B00000000,
        B00110000,
        B00110000,
        B01100000};
    alphabet[','] = {
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00110000,
        B00110000,
        B01100000};
    alphabet['!'] = {
        B00011000,
        B00111100,
        B00111100,
        B00011000,
        B00011000,
        B00000000,
        B00011000,
        B00000000};
    alphabet['?'] = {
        B01111000,
        B11001100,
        B00001100,
        B00011000,
        B00110000,
        B00000000,
        B00110000,
        B00000000};
    alphabet['@'] = {
        B01111100,
        B11000110,
        B11011110,
        B11011110,
        B11011110,
        B11000000,
        B01111000,
        B00000000};
    alphabet['&'] = {
        B00111000,
        B01101100,
        B00111000,
        B01110110,
        B11011100,
        B11001100,
        B01110110,
        B00000000};
    alphabet['$'] = {
        B00110000,
        B01111100,
        B11000000,
        B01111000,
        B00001100,
        B11111000,
        B00110000,
        B00000000};
    alphabet['#'] = {
        B01101100,
        B01101100,
        B11111110,
        B01101100,
        B11111110,
        B01101100,
        B01101100,
        B00000000};
    alphabet['\\'] = {
        B11000000,
        B01100000,
        B00110000,
        B00011000,
        B00001100,
        B00000110,
        B00000010,
        B00000000};
    alphabet['\''] = {
        B00110000,
        B00110000,
        B00011000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000};
    alphabet['|'] = {
        B00011000,
        B00011000,
        B00011000,
        B00011000,
        B00011000,
        B00011000,
        B00011000,
        B00011000};
    alphabet['"'] = {
        B01101100,
        B01101100,
        B01001000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000};
    alphabet['_'] = {
        B01101100,
        B01101100,
        B01001000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B01111110};

    return alphabet;
}

std::map<char, letterArray> alphabet = create_alphabet();

unsigned char *LetterAnimationClass::printNextFrame()
{
    clear();
    return alphabet[letter].data();
}