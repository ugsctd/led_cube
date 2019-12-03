
#ifndef _ALPHABET_H_
#define _ALPHABET_H_
#include <stdint.h>
#include <map>
using namespace std;

typedef array<unsigned char, 8> letterArray;
typedef std::map<char, letterArray> ledArray;

class AlphabetClass
{
public:
    AlphabetClass();
    ledArray alphabet;

};

extern AlphabetClass Alphabet;

#endif /* _ALPHABET_H_ */
