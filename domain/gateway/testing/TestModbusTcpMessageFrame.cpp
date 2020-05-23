#include "domain/gateway/includes/ModbusTcpMessageFrame.hpp"

#include "gtest/gtest.h"

namespace {

using namespace Gateway;

class TestModbusTcpMessageFrame : public ::testing::Test
{
protected:
    std::unique_ptr<ModbusTcpMessageFrame> createTestObject()
    {
        auto testObj = std::make_unique<ModbusTcpMessageFrame>();

        // create test object with dummy values
        testObj->transactionIdentifier = 0x0001;
        testObj->protocolIdentifier = 0x0000;
        testObj->lengthField = 0x0006;
        testObj->unitIdentifier = 0xff;
        testObj->functionCode = 0x03;

        // data bytes will be set within each test case
        testObj->dataBytes = {};

        return std::move(testObj);
    }
};

TEST_F(TestModbusTcpMessageFrame, getStartAddress)
{
    auto testObj = createTestObject();

    // set start address
    testObj->dataBytes = {0x12, 0x34};

    auto startAddr = testObj->getStartAddress();
    EXPECT_EQ(startAddr, 0x1234);
}

TEST_F(TestModbusTcpMessageFrame, getNumberOfValuesToReadOrWrite)
{
    auto testObj = createTestObject();

    // set number of values to read following the start address
    testObj->dataBytes = {0x12, 0x34, 0x00, 0x05};

    auto nbValsRead = testObj->getNumberOfValuesToReadOrWrite();
    EXPECT_EQ(nbValsRead, 0x0005);
}

TEST_F(TestModbusTcpMessageFrame, getSingleValueToWrite)
{
    auto testObj = createTestObject();

    // set single value to write following the start address
    testObj->dataBytes = {0x12, 0x34, 0x56, 0x78};

    auto valToWrite = testObj->getSingleValueToWrite();
    EXPECT_EQ(valToWrite, 0x5678);
}

TEST_F(TestModbusTcpMessageFrame, extractBitValues)
{
    auto testObj = createTestObject();

    // set bit values to rextract
    testObj->dataBytes = {0b10001100, 0b00011010};

    auto bitValues = testObj->extractBitValues(0, 0xd);
    std::vector<uint8_t> expectedBitValues = {0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1};
    EXPECT_EQ(bitValues, expectedBitValues);
}

TEST_F(TestModbusTcpMessageFrame, extractRegisterValues)
{
    auto testObj = createTestObject();

    // set 8 bytes of register values which represent 4 register
    testObj->dataBytes = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

    auto registerValues = testObj->extractRegisterValues(0, 4);
    std::vector<uint16_t> expectedRegisterValues = {0x1122, 0x3344, 0x5566, 0x7788};
    EXPECT_EQ(registerValues, expectedRegisterValues);
}

TEST_F(TestModbusTcpMessageFrame, asByteVector)
{
    auto testObj = createTestObject();
    testObj->dataBytes = {0x01, 0x02, 0x03, 0x04};

    auto testObjAsVector = testObj->asByteVector();
    std::vector<uint8_t> expectedVector = {0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0xff, 0x03, 0x01, 0x02, 0x03, 0x04};

    EXPECT_EQ(testObjAsVector, expectedVector);
}

TEST_F(TestModbusTcpMessageFrame, initFromByteVector)
{
    std::vector<uint8_t> testObjVector = {0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0xff, 0x03, 0x01, 0x02, 0x03, 0x04};
    auto testObj = ModbusTcpMessageFrame();
    testObj.initFromByteVector(testObjVector);

    auto expectedObj = createTestObject();
    expectedObj->dataBytes = {0x01, 0x02, 0x03, 0x04};

    EXPECT_EQ(testObj, *expectedObj);
}

/*TEST_F(TestModbusTcpMessageFrame, getNumberOfBytesOfValuesToWrite)
{
    auto testObj = createTestObject();

    // set number of bytes to write following the start address and the number of values to write
    testObj->dataBytes = {0x12, 0x34, 0x00, 0x04, 0x08};

    auto valToWrite = testObj->getNumberOfBytesOfValuesToWrite();
    EXPECT_EQ(valToWrite, 0x08);
}

TEST_F(TestModbusTcpMessageFrame, getNumberOfBytesOfReadValues)
{
    auto testObj = createTestObject();

    // set number of bytes of read values
    testObj->dataBytes = {0x04};

    auto bytesOfReadVals = testObj->getNumberOfBytesOfReadValues();
    EXPECT_EQ(bytesOfReadVals, 0x04);
}*/

/*TEST_F(TestModbusTcpMessageFrame, getReadBitValues)
{
    auto testObj = createTestObject();

    // read 2 bytes of coil values (1 bit represents a single coil)
    testObj->dataBytes = {0x02, 0b10001100, 0b00011010};

    auto readCoilValues = testObj->getReadBitValues();
    std::vector<uint8_t> expectedReadCoilValues = {0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1};
    EXPECT_EQ(readCoilValues, expectedReadCoilValues);
}

TEST_F(TestModbusTcpMessageFrame, getReadRegisterValues)
{
    auto testObj = createTestObject();

    // read 8 bytes of register values which represent 4 register
    testObj->dataBytes = {0x08, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

    auto readRegisterValues = testObj->getReadRegisterValues();
    std::vector<uint16_t> expectedReadRegisterValues = {0x1122, 0x3344, 0x5566, 0x7788};
    EXPECT_EQ(readRegisterValues, expectedReadRegisterValues);
}*/

}
