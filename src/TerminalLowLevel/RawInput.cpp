#include "RawInput.h"
#include "ANSISequences.h"
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

RawInput::RawInput() {
    if (tcgetattr(STDIN_FILENO, &this->originalSettings) < 0) {
        perror("Failed to read original terminal settings");
    }

    struct termios newSettings = originalSettings;

    newSettings.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    newSettings.c_oflag &= ~(OPOST);

    newSettings.c_cflag |= (CS8);

    newSettings.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    newSettings.c_cc[VMIN] = 5; newSettings.c_cc[VTIME] = 1;
    newSettings.c_cc[VMIN] = 0; newSettings.c_cc[VTIME] = 0;
    newSettings.c_cc[VMIN] = 2; newSettings.c_cc[VTIME] = 0;
    newSettings.c_cc[VMIN] = 0; newSettings.c_cc[VTIME] = 8;

    /* put terminal in raw mode after flushing */
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &newSettings) < 0) {
        perror("Failed to set terminal into raw mode");
    }
}

RawInput::~RawInput() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalSettings) < 0) {
        perror("Failed to restore terminal back to original mode");
    }
}

int RawInput::readKey() {
    uint8_t result;
    size_t bytesRead = read(STDIN_FILENO, &result, sizeof(result));

    if (bytesRead <= 0) {
        if (bytesRead < 0) {
            perror("Failed to read from standard input");
        }
        return -1;
    }

    return result;
}

bool RawInput::readANSISequence(ANSISequence& result) {
    ANSISequenceReader reader;
    while (1) {
        int ch = readKey();
        if (ch == -1) {
            return false;
        }
        switch (reader.feedByte(ch)) {
            case ANSISequenceReader::NEED_MORE_BYTES:
                continue;
            case ANSISequenceReader::COMPLETED_SEQUENCE:
                result = reader.getSequence();
                return true;
            case ANSISequenceReader::INVALID_SEQUENCE:
                return false;
        }
    }

}
