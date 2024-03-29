#include <stdint.h>
#include "static_pictures.h"
#include <binary.h>

#include <map>

PicturesClass Pictures = PicturesClass();

PicturesClass::PicturesClass()
{
    ledArray alphabet;
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

    this->alphabet = alphabet;
}
