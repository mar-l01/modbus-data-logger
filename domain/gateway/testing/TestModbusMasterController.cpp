#include "domain/entity/includes/ModbusTcpConstants.hpp"
#include "domain/gateway/includes/ModbusMasterControllerImpl.hpp"
#include "domain/gateway/testing/gmock/MockModbusMaster.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using namespace ::testing;
using namespace Gateway;
using namespace Entity;

class TestModbusMasterControllerImpl : public ::testing::Test
{
protected:
    TestModbusMasterControllerImpl()
        : m_modbusMasterMock(std::make_shared<MockModbusMaster>())
        , m_ipAddr("127.0.0.1")
        , m_port(502)
    {}

    std::shared_ptr<ModbusMasterControllerImpl> createTestObject()
    {
        auto testObj = std::make_shared<ModbusMasterControllerImpl>(m_modbusMasterMock, m_ipAddr, m_port);

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

TEST_F(TestModbusMasterControllerImpl, ctorSuccessful)
{
    EXPECT_NO_THROW(createTestObject());
}

TEST_F(TestModbusMasterControllerImpl, connect)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_modbusMasterMock, connect(m_ipAddr, m_port)).Times(1);
    testObj->connect();
}

TEST_F(TestModbusMasterControllerImpl, getExternalModbusSlaveResponseFc1)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::READ_COIL_VALUES);

    EXPECT_CALL(*m_modbusMasterMock, readCoilValues(mbReq->getStartAddress(), mbReq->getNumberOfValuesToReadOrWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterControllerImpl, getExternalModbusSlaveResponseFc2)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::READ_DISCRETE_INPUT_VALUES);

    EXPECT_CALL(*m_modbusMasterMock,
                readDiscreteInputValues(mbReq->getStartAddress(), mbReq->getNumberOfValuesToReadOrWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterControllerImpl, getExternalModbusSlaveResponseFc3)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::READ_HOLDING_REGISTER_VALUES);

    EXPECT_CALL(*m_modbusMasterMock,
                readHoldingRegisterValues(mbReq->getStartAddress(), mbReq->getNumberOfValuesToReadOrWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterControllerImpl, getExternalModbusSlaveResponseFc4)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::READ_INPUT_REGISTER_VALUES);

    EXPECT_CALL(*m_modbusMasterMock,
                readInputRegisterValues(mbReq->getStartAddress(), mbReq->getNumberOfValuesToReadOrWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterControllerImpl, getExternalModbusSlaveResponseFc5)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::WRITE_SINGLE_COIL_VALUE);

    EXPECT_CALL(*m_modbusMasterMock, writeSingleCoilValue(mbReq->getStartAddress(), mbReq->getSingleValueToWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterControllerImpl, getExternalModbusSlaveResponseFc6)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::WRITE_SINGLE_HOLDING_REGISTER_VALUE);

    EXPECT_CALL(*m_modbusMasterMock,
                writeSingleHoldingRegisterValue(mbReq->getStartAddress(), mbReq->getSingleValueToWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterControllerImpl, getExternalModbusSlaveResponseFc16)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::WRITE_MULTIPLE_COIL_VALUES);

    EXPECT_CALL(*m_modbusMasterMock, writeMultipleCoilValues(mbReq->getStartAddress(), mbReq->getCoilValuesToWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterControllerImpl, getExternalModbusSlaveResponseFc15)
{
    auto testObj = createTestObject();
    auto mbReq =
      createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::WRITE_MULTIPLE_HOLDING_REGISTER_VALUES);

    EXPECT_CALL(*m_modbusMasterMock,
                writeMultipleHoldingRegisterValues(mbReq->getStartAddress(), mbReq->getHoldingRegisterValuesToWrite()))
      .Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterControllerImpl, reconnectIfTimeout)
{
    auto testObj = createTestObject();
    auto mbReq = createModbusTcpRequestWithGivenFunctionCode(ModbusFunctionCode::READ_HOLDING_REGISTER_VALUES);

    EXPECT_CALL(*m_modbusMasterMock,
                readHoldingRegisterValues(mbReq->getStartAddress(), mbReq->getNumberOfValuesToReadOrWrite()))
      .Times(1)
      .WillOnce(Return(ModbusReadOperationResult<uint16_t>(ModbusOperationStatus::TIMEOUT, std::vector<uint16_t>())));
    EXPECT_CALL(*m_modbusMasterMock, close()).Times(1);
    EXPECT_CALL(*m_modbusMasterMock, reconnect()).Times(1);
    testObj->getExternalModbusSlaveResponse(mbReq);
}

TEST_F(TestModbusMasterControllerImpl, closeConnection)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_modbusMasterMock, close()).Times(1);
    testObj->disconnect();
}

}
