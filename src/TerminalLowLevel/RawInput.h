#ifndef RAWINPUT_H
#define RAWINPUT_H

#include <termios.h>
#include "ANSISequences.h"

class RawInput {
public:
    RawInput();
    ~RawInput();

    int readKey();
    bool readANSISequence(ANSISequence& result);

private:
    struct termios originalSettings;

};
#endif
