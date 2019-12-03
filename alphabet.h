
#ifndef _ALPHABET_H_
#define _ALPHABET_H_
#include <stdint.h>
#include <map>
using namespace std;

typedef array<unsigned char, 8> letterArray;
typedef std::map<char, letterArray> ledArray;

// std::map<char, letterArray> alphabet;

class AlphabetClass
{
public:
    AlphabetClass();
    // std::map<char, letterArray> alphabet;

    ledArray alphabet;

    // std::map<char, letterArray> create_alphabet();
};

extern AlphabetClass Alphabet;

#endif /* _ALPHABET_H_ */
