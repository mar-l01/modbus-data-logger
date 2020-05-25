#include "domain/gateway/includes/ModbusGateway.hpp"
#include "domain/gateway/testing/gmock/MockModbusResponseController.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using namespace ::testing;
using namespace Gateway;
using namespace Entity;

class TestModbusGateway : public ::testing::Test
{
protected:
    TestModbusGateway()
        : m_modbusResponseControllerMock(std::make_shared<MockModbusResponseController>())
    {}

    std::shared_ptr<ModbusGateway> createTestObject()
    {
        auto testObj = std::make_shared<ModbusGateway>(m_modbusResponseControllerMock);

        return testObj;
    }

    std::shared_ptr<MockModbusResponseController> m_modbusResponseControllerMock;
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

    EXPECT_CALL(*m_modbusResponseControllerMock, getExternalModbusSlaveResponse(mbTcpReq)).Times(1);
    auto mbTcpResp = testObj->forwardModbusRequestAndWaitForResponse(mbTcpReq);
}

}
