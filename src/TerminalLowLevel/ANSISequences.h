#ifndef ANSISEQUENCES_H
#define ANSISEQUENCES_H
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <memory>

class ANSISequenceReaderImpl;

class ANSISequence {
public:
    void write(std::ostream& os) const;
    ANSISequence build(char commandType, char prefix, std::vector<int> params, char function);
    ANSISequence buildCSI(std::vector<int> params, char function) {
        return build(ANSISequence::CSI_PREFIX, 0, params, function);
    }

    char getFunction() const { return func; }
    int getParam(size_t ix) const { return params[ix]; }

    static const char ESC = '\x1b';
    static const char CSI_PREFIX = '[';
    static const char DCS_PREFIX = 'P';
    static const char OSC_PREFIX = ']';

    static const char FUNC_CURSOR_MOVE_UP = 'A';
    static const char FUNC_CURSOR_MOVE_DOWN = 'B';
    static const char FUNC_CURSOR_MOVE_RIGHT = 'C';
    static const char FUNC_CURSOR_MOVE_LEFT = 'D';
    static const char FUNC_CURSOR_MOVE_DOWN_FIRSTCOL = 'E';
    static const char FUNC_CURSOR_MOVE_UP_FIRSTCOL = 'F';
    static const char FUNC_CURSOR_MOVE_TO_COLUMN = 'G';
    static const char FUNC_CURSOR_MOVE_HOME = 'H';
    static const char FUNC_CURSOR_MOVE_TO = 'H';

    static const char FUNC_ERASE_SCREEN = 'J';
    static const char FUNC_ERASE_LINE = 'K';
    static const int FUNC_ERASE_UNTIL_END = 0;
    static const int FUNC_ERASE_UNTIL_BEGINNING = 1;
    static const int FUNC_ERASE_ALL = 2;

    static const char FUNC_SGR = 'm';
    static const char FUNC_QUERY = 'n';

    static const int FUNC_SGR_BOLD = 1;
    static const int FUNC_SG_PAL24BIT = 2;
    static const int FUNC_SGR_ITALIC = 3;
    static const int FUNC_SGR_UNDERLINE = 4;
    static const int FUNC_SGR_PAL8BIT = 5;
    static const int FUNC_SGR_FOREGROUND_COLOR = 30;
    static const int FUNC_SGR_BACKGROUND_COLOR = 40;
    static const int FUNC_SGR_EXTPAL = 8;

    static const int FUNC_QUERY_TERMINAL_SIZE = 6;

private:
    
    static const size_t MAX_PARAMS = 8;
    int params[MAX_PARAMS];
    char func;
    char commandType;
    char prefix;

    friend class ANSISequenceReaderImpl;
};

namespace ANSISequences {
    inline ANSISequence CursorMove(int dx, int dy) {
        if (dx < 0) {
            return ANSISequence().buildCSI({-dx}, ANSISequence::FUNC_CURSOR_MOVE_LEFT);
        } else if (dx > 0) {
            return ANSISequence().buildCSI({dx}, ANSISequence::FUNC_CURSOR_MOVE_RIGHT);
        } else if (dy < 0) {
            return ANSISequence().buildCSI({-dy}, ANSISequence::FUNC_CURSOR_MOVE_UP);
        } else {
            return ANSISequence().buildCSI({dy}, ANSISequence::FUNC_CURSOR_MOVE_DOWN);
        }
    }

    inline ANSISequence ClearScreen() {
        return ANSISequence().buildCSI({ANSISequence::FUNC_ERASE_ALL}, ANSISequence::FUNC_ERASE_SCREEN);
    }

    inline ANSISequence GetTerminalDimensions() {
        return ANSISequence().buildCSI({ANSISequence::FUNC_QUERY_TERMINAL_SIZE}, ANSISequence::FUNC_QUERY);
    }

    inline ANSISequence CursorSet(int x, int y) {
        return ANSISequence().build(ANSISequence::CSI_PREFIX, 0, {y+1, x+1}, ANSISequence::FUNC_CURSOR_MOVE_TO);
    }
    inline ANSISequence TextColor(int color) {
        return ANSISequence().build(ANSISequence::CSI_PREFIX, 0, {
                (color >= 8) ? ANSISequence::FUNC_SGR_BOLD : 0,
                ANSISequence::FUNC_SGR_FOREGROUND_COLOR + (color & 0x07)}, ANSISequence::FUNC_SGR);
    }
    inline ANSISequence TextBackground(int color) {
        return ANSISequence().build(ANSISequence::CSI_PREFIX, 0, {
                ANSISequence::FUNC_SGR_BACKGROUND_COLOR + (color & 0x07)}, ANSISequence::FUNC_SGR);
    }
    inline ANSISequence TextAttribs(int bgColor, int fgColor) {
        return ANSISequence().build(ANSISequence::CSI_PREFIX, 0, {
                (fgColor >= 8) ? ANSISequence::FUNC_SGR_BOLD : 0,
                ANSISequence::FUNC_SGR_FOREGROUND_COLOR + (fgColor & 0x07),
                ANSISequence::FUNC_SGR_BACKGROUND_COLOR + (bgColor & 0x07)}, ANSISequence::FUNC_SGR);
    }
    inline ANSISequence TextColor8(int color) {
        return ANSISequence().build(ANSISequence::CSI_PREFIX, 0, {
                ANSISequence::FUNC_SGR_EXTPAL + ANSISequence::FUNC_SGR_FOREGROUND_COLOR,
                ANSISequence::FUNC_SGR_PAL8BIT,
                color}, ANSISequence::FUNC_SGR);
    }
    inline ANSISequence TextBackground8(int color) {
        return ANSISequence().build(ANSISequence::CSI_PREFIX, 0, {
                ANSISequence::FUNC_SGR_EXTPAL + ANSISequence::FUNC_SGR_BACKGROUND_COLOR,
                ANSISequence::FUNC_SGR_PAL8BIT,
                color}, ANSISequence::FUNC_SGR);
    }
}

class ANSISequenceReader {
public:
    enum ReaderResult {
        NEED_MORE_BYTES = 0,
        COMPLETED_SEQUENCE,
        INVALID_SEQUENCE
    };

    ANSISequenceReader();
    ~ANSISequenceReader();

    ReaderResult feedByte(char ch);
    ANSISequence getSequence() const;

    ANSISequenceReader(const ANSISequenceReader&) = delete;
    ANSISequenceReader operator=(const ANSISequenceReader&) = delete;

private:

    ANSISequenceReaderImpl* impl;
};

std::ostream& operator<<(std::ostream& os, const ANSISequence& seq);

#endif //ANSISEQUENCES_H
