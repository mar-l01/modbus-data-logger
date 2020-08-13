#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include "gtest/gtest.h"

namespace {

using namespace Entity;

class TestModbusTcpResponse : public ::testing::Test
{
protected:
    std::unique_ptr<ModbusTcpResponse> createTestObject(
      ModbusOperationStatus mbOpState = ModbusOperationStatus::SUCCESS)
    {
        auto mbRequest = std::make_shared<ModbusTcpRequest>();
        auto testObj = std::make_unique<ModbusTcpResponse>(mbRequest, mbOpState);

        return std::move(testObj);
    }
};

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

TEST_F(TestModbusTcpResponse, getReadBitValuesEmpty)
{
    auto testObj = createTestObject();

    auto expectedEmptyBitVector = std::vector<uint8_t>();

    auto readBitValues = testObj->getReadBitValues();
    EXPECT_EQ(readBitValues, expectedEmptyBitVector);
}

TEST_F(TestModbusTcpResponse, getReadRegisterValuesEmpty)
{
    auto testObj = createTestObject();

    auto expectedEmptyRegisterVector = std::vector<uint16_t>();

    auto readRegisterValues = testObj->getReadRegisterValues();
    EXPECT_EQ(readRegisterValues, expectedEmptyRegisterVector);
}

TEST_F(TestModbusTcpResponse, getModbusOperationStatus_SUCCESS)
{
    auto testObj = createTestObject();

    auto expectedModbusOperationStatus = ModbusOperationStatus::SUCCESS;

    auto operationStatus = testObj->getModbusOperationStatus();
    EXPECT_EQ(operationStatus, expectedModbusOperationStatus);
}

TEST_F(TestModbusTcpResponse, getModbusOperationStatus_FAIL)
{
    auto testObj = createTestObject(ModbusOperationStatus::FAIL);

    auto expectedModbusOperationStatus = ModbusOperationStatus::FAIL;

    auto operationStatus = testObj->getModbusOperationStatus();
    EXPECT_EQ(operationStatus, expectedModbusOperationStatus);
}

TEST_F(TestModbusTcpResponse, getModbusOperationStatus_TIMEOUT)
{
    auto testObj = createTestObject(ModbusOperationStatus::TIMEOUT);

    auto expectedModbusOperationStatus = ModbusOperationStatus::TIMEOUT;

    auto operationStatus = testObj->getModbusOperationStatus();
    EXPECT_EQ(operationStatus, expectedModbusOperationStatus);
}

TEST_F(TestModbusTcpResponse, convertToLogString)
{
    // set-up test object with specific ModbusTcpRequest
    auto mbRequest = std::make_shared<ModbusTcpRequest>();
    mbRequest->transactionIdentifier = 4;
    mbRequest->protocolIdentifier = 0;
    mbRequest->lengthField = 6;
    mbRequest->unitIdentifier = 255;
    mbRequest->functionCode = 3;
    mbRequest->dataBytes = {0x00, 0x00, 0x00, 0x03};
    ModbusTcpResponse mbResponse(mbRequest, ModbusOperationStatus::SUCCESS);
    std::vector<uint16_t> readRegisterValues = {4660, 22136, 39612};
    mbResponse.setReadValues(readRegisterValues);

    std::string expectedOutput(
      "<--- Modbus Response --- \n<0004><0000><0009><ff><03><06><12><34><56><78><9a><bc>\n\tTransaction Id: "
      "4\n\tProtocol Id: 0 (TCP/IP Protocol)\n\tLength: 9\n\tUnit Id: 255\n\tFunction Code: "
      "READ_HOLDING_REGISTER_VALUES\n\tData Bytes:\n\t|--Number of bytes to follow: 6\n\t|--Holding Register (1): "
      "4660\n\t|--Holding Register (2): 22136\n\t|--Holding Register (3): 39612\n");

    auto generatedLogOutput = mbResponse.convertToLogString();

    EXPECT_STRCASEEQ(generatedLogOutput.c_str(), expectedOutput.c_str());
}

}
