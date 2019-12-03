
#ifndef _ALPHABET_H_
#define _ALPHABET_H_
#include <stdint.h>
#include <map>
using namespace std;

typedef array<unsigned char, 8> letterArray;
typedef std::map<char, letterArray> ledArray;

class PicturesClass
{
public:
    PicturesClass();
    ledArray alphabet;

};

extern PicturesClass Pictures;

#endif /* _ALPHABET_H_ */
