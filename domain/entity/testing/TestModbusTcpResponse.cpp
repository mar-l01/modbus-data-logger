#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include "gtest/gtest.h"

namespace {

using namespace Entity;

class TestModbusTcpResponse : public ::testing::Test
{
protected:
    std::unique_ptr<ModbusTcpResponse> createTestObject()
    {
        auto testObj = std::make_unique<ModbusTcpResponse>();

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

TEST_F(TestModbusTcpResponse, getNumberOfBytesOfReadValues)
{
    auto testObj = createTestObject();

    // set number of bytes of read values
    testObj->dataBytes = {0x04};

    auto bytesOfReadVals = testObj->getNumberOfBytesOfReadValues();
    EXPECT_EQ(bytesOfReadVals, 0x04);
}

TEST_F(TestModbusTcpResponse, getReadBitValues)
{
    auto testObj = createTestObject();

    // 1 bit represents a single coil value
    std::vector<uint8_t> expectedReadCoilValues = {1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0};
    testObj->setReadValues(expectedReadCoilValues);

    auto readCoilValues = testObj->getReadBitValues();
    EXPECT_EQ(readCoilValues, expectedReadCoilValues);
}

TEST_F(TestModbusTcpResponse, getReadRegisterValues)
{
    auto testObj = createTestObject();

    // read 8 bytes of register values which represent 4 register
    std::vector<uint16_t> expectedReadRegisterValues = {0x1122, 0x3344, 0x5566, 0x7788};
    testObj->setReadValues(expectedReadRegisterValues);

    auto readRegisterValues = testObj->getReadRegisterValues();
    EXPECT_EQ(readRegisterValues, expectedReadRegisterValues);
}
}
