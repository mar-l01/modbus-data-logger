#include "domain/gateway/includes/ModbusGateway.hpp"
#include "domain/gateway/testing/gmock/MockModbusResponseController.hpp"
#include "domain/logging/testing/gmock/MockDataLogger.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using namespace ::testing;
using namespace Gateway;
using namespace Entity;
using namespace Logging;

class TestModbusGateway : public ::testing::Test
{
protected:
    TestModbusGateway()
        : m_modbusResponseControllerMock(std::make_shared<MockModbusResponseController>())
        , m_dataLoggerMock(std::make_shared<MockDataLogger>())
    {}

    std::shared_ptr<ModbusGateway> createTestObject()
    {
        return std::make_shared<ModbusGateway>(m_modbusResponseControllerMock, m_dataLoggerMock);
    }

    std::shared_ptr<MockModbusResponseController> m_modbusResponseControllerMock;
    std::shared_ptr<MockDataLogger> m_dataLoggerMock;
};

TEST_F(TestModbusGateway, ctorSuccessful)
{
    EXPECT_NO_THROW(createTestObject());
}

TEST_F(TestModbusGateway, forwardModbusRequestAndWaitForResponse)
{
    auto testObj = createTestObject();

    auto mbTcpReq = std::make_shared<ModbusTcpRequest>();
    mbTcpReq->transactionIdentifier = 0x0001;
    mbTcpReq->protocolIdentifier = 0x0000;
    mbTcpReq->lengthField = 0x0006;
    mbTcpReq->unitIdentifier = 0xff;
    mbTcpReq->functionCode = 0x01;
    mbTcpReq->dataBytes = {0x12, 0x34, 0x00, 0x05};

    auto mbTcpReturnedResp = std::make_shared<ModbusTcpResponse>();

    EXPECT_CALL(*m_dataLoggerMock, logModbusRequest(_)).Times(1);
    EXPECT_CALL(*m_modbusResponseControllerMock, getExternalModbusSlaveResponse(mbTcpReq))
      .WillOnce(Return(mbTcpReturnedResp));
    EXPECT_CALL(*m_dataLoggerMock, logModbusResponse(_)).Times(1);

    auto mbTcpResp = testObj->forwardModbusRequestAndWaitForResponse(mbTcpReq);
    EXPECT_EQ(*mbTcpResp, *mbTcpReturnedResp);
}

TEST_F(TestModbusGateway, callForLoggingOfModbusRequestAndResponse)
{
    auto testObj = createTestObject();

    auto mbTcpReq = std::make_shared<ModbusTcpRequest>();
    mbTcpReq->transactionIdentifier = 0x0001;
    mbTcpReq->protocolIdentifier = 0x0000;
    mbTcpReq->lengthField = 0x0006;
    mbTcpReq->unitIdentifier = 0xff;
    mbTcpReq->functionCode = 0x01;
    mbTcpReq->dataBytes = {0x12, 0x34, 0x00, 0x05};

    auto mbTcpRes = std::make_shared<ModbusTcpResponse>();

    EXPECT_CALL(*m_dataLoggerMock, logModbusRequest(mbTcpReq)).Times(1);
    EXPECT_CALL(*m_modbusResponseControllerMock, getExternalModbusSlaveResponse(_)).WillOnce(Return(mbTcpRes));
    EXPECT_CALL(*m_dataLoggerMock, logModbusResponse(mbTcpRes)).Times(1);

    auto mbTcpResponse = testObj->forwardModbusRequestAndWaitForResponse(mbTcpReq);
    EXPECT_EQ(*mbTcpRes, *mbTcpResponse);
}

}
