#include "domain/entity/includes/ModbusTcpConstants.hpp"
#include "domain/gateway/includes/ModbusMasterController.hpp"
#include "domain/gateway/testing/gmock/MockModbusMaster.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using namespace ::testing;
using namespace Gateway;
using namespace Entity;

class TestModbusMasterController : public ::testing::Test
{
protected:
    TestModbusMasterController()
        : m_modbusMasterMock(std::make_shared<MockModbusMaster>())
        , m_ipAddr("127.0.0.1")
        , m_port(502)
    {}

    std::shared_ptr<ModbusMasterController> createTestObject()
    {
        auto testObj = std::make_shared<ModbusMasterController>(m_modbusMasterMock, m_ipAddr, m_port);

        return testObj;
    }

    std::shared_ptr<ModbusTcpRequest> createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode fc)
    {
        auto functionCode = static_cast<uint8_t>(fc);

        // define dummy ModbusTcpRequest (FC and dataBytes of only importance)
        auto mbTcpReq = std::make_unique<ModbusTcpRequest>();
        mbTcpReq->transactionIdentifier = 0x0001;
        mbTcpReq->protocolIdentifier = 0x0000;
        mbTcpReq->lengthField = 0x0006;
        mbTcpReq->unitIdentifier = 0xff;
        mbTcpReq->functionCode = functionCode; // only required value

        // define data-bytes necessary for functions tested in this test-suite
        switch (functionCode) {
            case static_cast<uint8_t>(ModbusFunctionCode::READ_COIL_VALUES): {
                mbTcpReq->dataBytes = {0x12, 0x34, 0x00, 0x05};
                break;
            }
            case static_cast<uint8_t>(ModbusFunctionCode::READ_DISCRETE_INPUT_VALUES): {
                mbTcpReq->dataBytes = {0x12, 0x34, 0x00, 0x05};
                break;
            }
            case static_cast<uint8_t>(ModbusFunctionCode::READ_HOLDING_REGISTER_VALUES): {
                mbTcpReq->dataBytes = {0x12, 0x34, 0x00, 0x05};
                break;
            }
            case static_cast<uint8_t>(ModbusFunctionCode::READ_INPUT_REGISTER_VALUES): {
                mbTcpReq->dataBytes = {0x12, 0x34, 0x00, 0x05};
                break;
            }
            case static_cast<uint8_t>(ModbusFunctionCode::WRITE_SINGLE_COIL_VALUE): {
                mbTcpReq->dataBytes = {0x12, 0x34, 0x00, 0x00};
                break;
            }
            case static_cast<uint8_t>(ModbusFunctionCode::WRITE_SINGLE_HOLDING_REGISTER_VALUE): {
                mbTcpReq->dataBytes = {0x12, 0x34, 0x00, 0x00};
                break;
            }
            case static_cast<uint8_t>(ModbusFunctionCode::WRITE_MULTIPLE_COIL_VALUES): {
                mbTcpReq->dataBytes = {0x12, 0x34, 0x00, 0x02, 0x01, 0x00};
                break;
            }
            case static_cast<uint8_t>(ModbusFunctionCode::WRITE_MULTIPLE_HOLDING_REGISTER_VALUES): {
                mbTcpReq->dataBytes = {0x12, 0x34, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00, 0x01};
                break;
            }
            default:
                mbTcpReq->dataBytes = {};
                break;
        }

        return mbTcpReq;
    }

    std::shared_ptr<MockModbusMaster> m_modbusMasterMock;
    std::string m_ipAddr;
    int m_port;
};

TEST_F(TestModbusMasterController, ctorSuccessful)
{
    EXPECT_NO_THROW(createTestObject());
}

TEST_F(TestModbusMasterController, connect)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_modbusMasterMock, connect(m_ipAddr, m_port)).Times(1);
    testObj->connect();
}

TEST_F(TestModbusMasterController, getExternalModbusSlaveResponseFc1)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::READ_COIL_VALUES);

    EXPECT_CALL(*m_modbusMasterMock, readCoilValues(mbReq->getStartAddress(), mbReq->getNumberOfValuesToReadOrWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterController, getExternalModbusSlaveResponseFc2)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::READ_DISCRETE_INPUT_VALUES);

    EXPECT_CALL(*m_modbusMasterMock,
                readDiscreteInputValues(mbReq->getStartAddress(), mbReq->getNumberOfValuesToReadOrWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterController, getExternalModbusSlaveResponseFc3)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::READ_HOLDING_REGISTER_VALUES);

    EXPECT_CALL(*m_modbusMasterMock,
                readHoldingRegisterValues(mbReq->getStartAddress(), mbReq->getNumberOfValuesToReadOrWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterController, getExternalModbusSlaveResponseFc4)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::READ_INPUT_REGISTER_VALUES);

    EXPECT_CALL(*m_modbusMasterMock,
                readInputRegisterValues(mbReq->getStartAddress(), mbReq->getNumberOfValuesToReadOrWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterController, getExternalModbusSlaveResponseFc5)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::WRITE_SINGLE_COIL_VALUE);

    EXPECT_CALL(*m_modbusMasterMock, writeSingleCoilValue(mbReq->getStartAddress(), mbReq->getSingleValueToWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterController, getExternalModbusSlaveResponseFc6)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::WRITE_SINGLE_HOLDING_REGISTER_VALUE);

    EXPECT_CALL(*m_modbusMasterMock,
                writeSingleHoldingRegisterValue(mbReq->getStartAddress(), mbReq->getSingleValueToWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterController, getExternalModbusSlaveResponseFc16)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::WRITE_MULTIPLE_COIL_VALUES);

    EXPECT_CALL(*m_modbusMasterMock, writeMultipleCoilValues(mbReq->getStartAddress(), mbReq->getCoilValuesToWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterController, getExternalModbusSlaveResponseFc15)
{
    auto testObj = createTestObject();
    auto mbReq =
      createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::WRITE_MULTIPLE_HOLDING_REGISTER_VALUES);

    EXPECT_CALL(*m_modbusMasterMock,
                writeMultipleHoldingRegisterValues(mbReq->getStartAddress(), mbReq->getHoldingRegisterValuesToWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterController, reconnectIfTimeout)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::READ_HOLDING_REGISTER_VALUES);

    EXPECT_CALL(*m_modbusMasterMock,
                readHoldingRegisterValues(mbReq->getStartAddress(), mbReq->getNumberOfValuesToReadOrWrite()))
      .WillOnce(Return(ModbusReadOperationResult<uint16_t>(ModbusOperationStatus::TIMEOUT, std::vector<uint16_t>())));
    EXPECT_CALL(*m_modbusMasterMock, close()).Times(1);
    EXPECT_CALL(*m_modbusMasterMock, reconnect()).Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterController, closeConnection)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_modbusMasterMock, close()).Times(1);
    testObj->closeConnection();
}

}
