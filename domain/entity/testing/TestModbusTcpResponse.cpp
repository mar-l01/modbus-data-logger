#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include "gtest/gtest.h"

namespace {

using namespace Entity;

class TestModbusTcpResponse : public ::testing::Test
{
protected:
    std::unique_ptr<ModbusTcpResponse> createTestObject(bool useBits = false)
    {
        // arguments required for response object
        auto mbTcpReq = std::make_shared<ModbusTcpRequest>();
        mbTcpReq->transactionIdentifier = 0x0001;
        mbTcpReq->protocolIdentifier = 0x0000;
        mbTcpReq->lengthField = 0x0006;
        mbTcpReq->unitIdentifier = 0xff;
        mbTcpReq->functionCode = 0x03;

        if (useBits) {
            mbTcpReq->dataBytes = {0x12, 0x34, 0x00, 0x0d};
        } else {
            mbTcpReq->dataBytes = {0x12, 0x34, 0x00, 0x04};
        }
        std::vector<uint8_t> dummyVec = {0, 0, 0, 0, 0, 5, 0, 0};

        auto testObj = std::make_unique<ModbusTcpResponse>(dummyVec, mbTcpReq);

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
    auto testObj = createTestObject(true);

    // read 2 bytes of coil values (1 bit represents a single coil)
    testObj->dataBytes = {0x02, 0b10001100, 0b00011010};

    auto readCoilValues = testObj->getReadBitValues();
    std::vector<uint8_t> expectedReadCoilValues = {0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1};
    EXPECT_EQ(readCoilValues, expectedReadCoilValues);
}

TEST_F(TestModbusTcpResponse, getReadRegisterValues)
{
    auto testObj = createTestObject();

    // read 8 bytes of register values which represent 4 register
    testObj->dataBytes = {0x08, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

    auto readRegisterValues = testObj->getReadRegisterValues();
    std::vector<uint16_t> expectedReadRegisterValues = {0x1122, 0x3344, 0x5566, 0x7788};
    EXPECT_EQ(readRegisterValues, expectedReadRegisterValues);
}

}
