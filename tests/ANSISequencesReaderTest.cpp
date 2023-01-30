#include "ANSISequences.h"
#include "gtest/gtest.h"

TEST(ANSISequenceReader, SuccessfulCase1) {
    ANSISequenceReader reader;

    EXPECT_EQ(ANSISequenceReader::NEED_MORE_BYTES, reader.feedByte('\x1b'));
    EXPECT_EQ(ANSISequenceReader::NEED_MORE_BYTES, reader.feedByte('['));
    EXPECT_EQ(ANSISequenceReader::COMPLETED_SEQUENCE, reader.feedByte('K'));

    ANSISequence seq = reader.getSequence();
    std::ostringstream os;
    os << seq;
    EXPECT_EQ("\x1b[K", os.str());
}

TEST(ANSISequenceReader, SuccessfulCase2) {
    ANSISequenceReader reader;

    EXPECT_EQ(ANSISequenceReader::NEED_MORE_BYTES, reader.feedByte('\x1b'));
    EXPECT_EQ(ANSISequenceReader::NEED_MORE_BYTES, reader.feedByte('['));
    EXPECT_EQ(ANSISequenceReader::NEED_MORE_BYTES, reader.feedByte('3'));
    EXPECT_EQ(ANSISequenceReader::COMPLETED_SEQUENCE, reader.feedByte('K'));
    ANSISequence seq = reader.getSequence();
    std::ostringstream os;
    os << seq;
    EXPECT_EQ("\x1b[3K", os.str());
}

TEST(ANSISequenceReader, SuccessfulCase3) {
    ANSISequenceReader reader;

    EXPECT_EQ(ANSISequenceReader::NEED_MORE_BYTES, reader.feedByte('\x1b'));
    EXPECT_EQ(ANSISequenceReader::NEED_MORE_BYTES, reader.feedByte('['));
    EXPECT_EQ(ANSISequenceReader::NEED_MORE_BYTES, reader.feedByte('2'));
    EXPECT_EQ(ANSISequenceReader::NEED_MORE_BYTES, reader.feedByte('1'));
    EXPECT_EQ(ANSISequenceReader::NEED_MORE_BYTES, reader.feedByte(';'));
    EXPECT_EQ(ANSISequenceReader::NEED_MORE_BYTES, reader.feedByte('3'));
    EXPECT_EQ(ANSISequenceReader::NEED_MORE_BYTES, reader.feedByte('7'));
    EXPECT_EQ(ANSISequenceReader::COMPLETED_SEQUENCE, reader.feedByte('K'));
    
    ANSISequence seq = reader.getSequence();
    std::ostringstream os;
    os << seq;
    EXPECT_EQ("\x1b[21;37K", os.str());
}
