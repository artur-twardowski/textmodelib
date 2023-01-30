#include <iostream>
#include "ANSISequences.h"
#include "RawInput.h"

namespace AF = ANSISequences;

int main(void) {
    RawInput rawInput;
    ANSISequence dimensions;

    std::cout << AF::CursorSet(999, 999) << AF::GetTerminalDimensions();
    std::cout.flush();
    if (!rawInput.readANSISequence(dimensions)) {
        std::cout << "Failed to read terminal dimensions!" << std::endl;
        return 1;
    }

    std::cout << AF::ClearScreen();
    std::cout << AF::CursorSet(0, 0) << AF::TextAttribs(0, 15) << "Terminal size: "
        << dimensions.getParam(1) << "x" << dimensions.getParam(0);
    std::cout << AF::CursorSet(1, 2) << AF::TextAttribs(0, 15);
    std::cout << "16-color palette:";

    std::cout << AF::CursorSet(1, 12);
    std::cout << "16-color dithered palette:";
    for (int bgcolor = 0; bgcolor < 8; bgcolor++) {
        std::cout << AF::CursorSet(1, 3 + bgcolor);
        for (int fgcolor = 0; fgcolor < 16; fgcolor++) {
            std::cout << AF::TextAttribs(bgcolor, fgcolor) << " " << std::hex << bgcolor << fgcolor << " ";
        }

        std::cout << AF::CursorSet(1, 13 + bgcolor);
        for (int fgcolor = 0; fgcolor < 16; fgcolor++) {
            std::cout << AF::TextAttribs(bgcolor, fgcolor) << "\u2591\u2592\u2593\u2588";
        }
    }
    
    std::cout << AF::CursorSet(75, 2) << AF::TextAttribs(0, 15);
    std::cout << "256-color palette:";

    std::cout << AF::CursorSet(75, 3);
    for (int fgcolor = 0; fgcolor < 16; fgcolor++) {
        std::cout << AF::TextColor8(fgcolor) << "\u2588\u2588";
    }
    for (int fgcolorHi = 0; fgcolorHi < 6; fgcolorHi++) {
        std::cout << AF::CursorSet(75, 4 + fgcolorHi);
        for (int fgcolorLo = 0; fgcolorLo < 36; fgcolorLo++) {
            std::cout << AF::TextColor8(fgcolorHi * 36 + fgcolorLo + 16) << "\u2588";
        }
    }
    std::cout << AF::CursorSet(75, 10);
    for (int fgcolor = 6*6*6+16; fgcolor < 256; fgcolor++) {
        std::cout << AF::TextColor8(fgcolor) << "\u2588";
    }
    
    std::cout << AF::CursorSet(0, 23);

    std::cout.flush();

    int key;
    while (1) {
        key = rawInput.readKey();
        if (key >= 33 && key <= 126) {
            std::cout << (char)key;
        } else if (key != -1) {
            std::cout << "<" << key << ">";
        }
        std::cout.flush();
        if (key == 'x') {
            break;
        }
    }
    return 0;
}
