#include "ANSISequences.h"
#include "gtest/gtest.h"

TEST(ANSISequences, Building1) {
    ANSISequence seq;
    seq.build(ANSISequence::CSI_PREFIX, '\0', {15, 30}, ANSISequence::FUNC_CURSOR_MOVE_TO);

    std::ostringstream os;
    os << seq;
    EXPECT_EQ("\x1b[15;30H", os.str());
}
