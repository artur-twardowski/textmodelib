#include "ANSISequences.h"
#include "stdlib.h"
#include <iostream>

void ANSISequence::write(std::ostream& os) const {
    os << ESC << commandType;
    if (prefix) {
        os << prefix;
    }
    size_t paramIx = 0;
    while (params[paramIx] != -1) {
        os << std::dec << params[paramIx++];
        if (params[paramIx] == -1) {
            break;
        } else {
            os << ';';
        }
    }
    os << func;
}

ANSISequence ANSISequence::build(char commandType, char prefix, std::vector<int> params, char function) {
    this->commandType = commandType;
    this->prefix = prefix;
    this->func = function;

    size_t ix = 0;
    while (ix < params.size()) {
        this->params[ix] = params[ix];
        ix++;
    }
    while (ix < MAX_PARAMS) {
        this->params[ix] = -1;
        ix++;
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const ANSISequence& seq) {
    seq.write(os);
    return os;
}

class ANSISequenceReaderImpl {
public:
    enum class State {
        NOTHING_READ, READ_ESC, READ_COMMAND, READING_PARAMS, COMPLETED_READING, FAILED_READING
    };

    void feedByte(char ch);
    State state = State::NOTHING_READ;
    ANSISequence result;
    std::string param = "";
    size_t paramIx = 0;

    void decodeAndSaveParam();
};

ANSISequenceReader::ANSISequenceReader() {
    impl = new ANSISequenceReaderImpl();
    impl->result.build(0, 0, {}, 0);
}

ANSISequenceReader::~ANSISequenceReader() {
    delete impl;
}

void ANSISequenceReaderImpl::decodeAndSaveParam() {
    if (param.size() > 0) {
        int paramValue = strtol(param.c_str(), nullptr, 10);
        result.params[paramIx] = paramValue;
    }
}

void ANSISequenceReaderImpl::feedByte(char ch) {
    switch (state) {
        case State::NOTHING_READ:
            if (ch == ANSISequence::ESC) {
                state = State::READ_ESC;
            } else {
                state = State::FAILED_READING;
            }
            break;

        case State::READ_ESC:
            if (ch == ANSISequence::CSI_PREFIX || ch == ANSISequence::DCS_PREFIX || ch == ANSISequence::OSC_PREFIX) {
                state = State::READ_COMMAND;
                result.commandType = ch;
            } else {
                state = State::FAILED_READING;
            }
            break;

        case State::READ_COMMAND:
            if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
                decodeAndSaveParam();
                state = State::COMPLETED_READING;
                result.func = ch;
            } else if (ch >= '0' && ch <= '9') {
                state = State::READING_PARAMS;
                param += ch;
            } else {
                state = State::FAILED_READING;
            }
            break;

        case State::READING_PARAMS:
            if ((ch >= '0') && (ch <= '9')) {
                param += ch;
            } else if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
                decodeAndSaveParam();
                state = State::COMPLETED_READING;
                result.func = ch;
            } else if (ch == ';') {
                decodeAndSaveParam();
                param = "";
                paramIx++;
                state = State::READ_COMMAND;
            } else {
                state = State::FAILED_READING;
            }
        default:
            break;
    }
}

ANSISequenceReader::ReaderResult ANSISequenceReader::feedByte(char ch) {
    impl->feedByte(ch);

    if (impl->state == ANSISequenceReaderImpl::State::COMPLETED_READING) {
        return COMPLETED_SEQUENCE;
    } else if (impl->state == ANSISequenceReaderImpl::State::FAILED_READING) {
        return INVALID_SEQUENCE;
    } else {
        return NEED_MORE_BYTES;
    }
}

ANSISequence ANSISequenceReader::getSequence() const {
    return this->impl->result;
}

