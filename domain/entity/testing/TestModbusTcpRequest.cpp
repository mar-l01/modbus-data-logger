#include "domain/entity/includes/ModbusTcpRequest.hpp"

#include "gtest/gtest.h"

namespace {

using namespace Entity;

class TestModbusTcpRequest : public ::testing::Test
{
protected:
    std::unique_ptr<ModbusTcpRequest> createTestObject()
    {
        auto testObj = std::make_unique<ModbusTcpRequest>();

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

TEST_F(TestModbusTcpRequest, getNumberOfBytesOfValuesToWrite)
{
    auto testObj = createTestObject();

    // set number of bytes to write following the start address and the number of values to write
    testObj->dataBytes = {0x12, 0x34, 0x00, 0x04, 0x08};

    auto valToWrite = testObj->getNumberOfBytesOfValuesToWrite();
    EXPECT_EQ(valToWrite, 0x08);
}

TEST_F(TestModbusTcpRequest, getCoilValuesToWrite)
{
    auto testObj = createTestObject();

    // set bit values to rextract
    testObj->dataBytes = {0x12, 0x34, 0x00, 0x0d, 0x02, 0b10001100, 0b00011010};

    auto coilValues = testObj->getCoilValuesToWrite();
    std::vector<uint8_t> expectedCoilValues = {0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1};
    EXPECT_EQ(coilValues, expectedCoilValues);
}

TEST_F(TestModbusTcpRequest, getHoldingRegisterValuesToWrite)
{
    auto testObj = createTestObject();

    // set 8 bytes of register values which represent 4 register
    testObj->dataBytes = {0x12, 0x34, 0x00, 0x04, 0x08, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

    auto registerValues = testObj->getHoldingRegisterValuesToWrite();
    std::vector<uint16_t> expectedRegisterValues = {0x1122, 0x3344, 0x5566, 0x7788};
    EXPECT_EQ(registerValues, expectedRegisterValues);
}

TEST_F(TestModbusTcpRequest, convertToLogString)
{
    auto testObj = createTestObject();

    testObj->dataBytes = {0x00, 0x04, 0x00, 0x0a};

    std::string expectedOutput(
      "--- Modbus Request ---> \n<0001><0000><0006><ff><03><00><04><00><0a>\n\tTransaction Id: 1\n\tProtocol Id: 0 "
      "(TCP/IP Protocol)\n\tLength: 6\n\tUnit Id: 255\n\tFunction Code: READ_HOLDING_REGISTER_VALUES\n\tData "
      "Bytes:\n\t|--Start Address: 4\n\t|--Number of values to read: 10\n");

    auto generatedLogOutput = testObj->convertToLogString();

    EXPECT_STRCASEEQ(generatedLogOutput.c_str(), expectedOutput.c_str());
}

}
