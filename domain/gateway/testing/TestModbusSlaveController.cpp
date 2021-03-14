#include "domain/gateway/includes/ModbusSlaveControllerImpl.hpp"
#include "domain/gateway/testing/gmock/MockModbusRequestController.hpp"
#include "domain/gateway/testing/gmock/MockModbusSlave.hpp"
#include "domain/utility/testing/gmock/MockTimer.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using namespace ::testing;
using namespace Gateway;
using namespace Entity;

class TestModbusSlaveControllerImpl : public ::testing::Test
{
protected:
    TestModbusSlaveControllerImpl()
        : m_modbusSlaveMock(std::make_shared<MockModbusSlave>())
        , m_modbusRequestControllerMock(std::make_shared<MockModbusRequestController>())
        , m_timerMock(std::make_shared<MockTimer>())
        , m_modbusTcpRequest(std::make_shared<ModbusTcpRequest>())
        , m_modbusDataMapping({0, 0, 0, 0, 1, 2, 3, 4})
        , m_ipAddr("127.0.0.1")
        , m_port(502)
    {
        // set function code of received request (by mock) to be supported
        m_modbusTcpRequest->functionCode = Entity::ModbusFunctionCode::READ_HOLDING_REGISTER_VALUES;
    }

    std::shared_ptr<ModbusSlaveControllerImpl> createTestObject()
    {
        EXPECT_CALL(*m_modbusSlaveMock, setModbusDataMapping(_)).Times(1);
        EXPECT_CALL(*m_modbusSlaveMock, bind(m_ipAddr, m_port)).Times(1);
        auto testObj = std::make_shared<ModbusSlaveControllerImpl>(m_modbusSlaveMock, m_modbusRequestControllerMock,
                                                                   m_timerMock, m_modbusDataMapping, m_ipAddr, m_port);
        return testObj;
    }

    std::shared_ptr<MockModbusSlave> m_modbusSlaveMock;
    std::shared_ptr<MockModbusRequestController> m_modbusRequestControllerMock;
    std::shared_ptr<MockTimer> m_timerMock;
    std::shared_ptr<ModbusTcpRequest> m_modbusTcpRequest;
    ModbusDataMapping m_modbusDataMapping;
    std::string m_ipAddr;
    int m_port;
};

TEST_F(TestModbusSlaveControllerImpl, ctorSuccessful)
{
    EXPECT_NO_THROW(createTestObject());
}

TEST_F(TestModbusSlaveControllerImpl, connect)
{
    auto testObj = createTestObject();
    int modbusSocket = 1;

    EXPECT_CALL(*m_modbusSlaveMock, listen(1)).WillOnce(Return(modbusSocket));
    EXPECT_CALL(*m_modbusSlaveMock, accept(modbusSocket)).Times(1);
    testObj->waitForIncomingConnection();
}

TEST_F(TestModbusSlaveControllerImpl, runOneFullLoop)
{
    auto testObj = createTestObject();

    Sequence seq;
    EXPECT_CALL(*m_modbusSlaveMock, receive(_))
      .InSequence(seq)
      .WillOnce(DoAll(testing::SetArgReferee<0>(m_modbusTcpRequest), Return(Gateway::ModbusReceiveStatus::OK)));
    EXPECT_CALL(*m_timerMock, restart()).InSequence(seq);
    EXPECT_CALL(*m_modbusRequestControllerMock, forwardModbusRequestAndWaitForResponse(_))
      .InSequence(seq)
      .WillOnce(Return(std::make_shared<ModbusTcpResponse>(m_modbusTcpRequest, ModbusOperationStatus::SUCCESS)));
    EXPECT_CALL(*m_modbusSlaveMock, reply(_)).InSequence(seq).WillOnce(Return(Gateway::ModbusReceiveStatus::OK));
    EXPECT_CALL(*m_modbusSlaveMock, receive(_))
      .InSequence(seq)
      .WillOnce(DoAll(testing::SetArgReferee<0>(m_modbusTcpRequest),
                      Return(Gateway::ModbusReceiveStatus::FAILED))); // break loop
    EXPECT_CALL(*m_timerMock, restart()).InSequence(seq);
    testObj->run();
}

TEST_F(TestModbusSlaveControllerImpl, runReceiveLoop)
{
    auto testObj = createTestObject();

    Sequence seq;
    // 0 invokes the do-while loop again
    EXPECT_CALL(*m_modbusSlaveMock, receive(_))
      .InSequence(seq)
      .WillOnce(DoAll(testing::SetArgReferee<0>(m_modbusTcpRequest), Return(Gateway::ModbusReceiveStatus::IGNORED)));
    EXPECT_CALL(*m_timerMock, restart()).InSequence(seq);
    EXPECT_CALL(*m_modbusSlaveMock, receive(_))
      .InSequence(seq)
      .WillOnce(DoAll(testing::SetArgReferee<0>(m_modbusTcpRequest), Return(Gateway::ModbusReceiveStatus::IGNORED)));
    EXPECT_CALL(*m_timerMock, restart()).InSequence(seq);
    EXPECT_CALL(*m_modbusSlaveMock, receive(_))
      .InSequence(seq)
      .WillOnce(DoAll(testing::SetArgReferee<0>(m_modbusTcpRequest),
                      Return(Gateway::ModbusReceiveStatus::FAILED))); // break loop
    EXPECT_CALL(*m_timerMock, restart()).InSequence(seq);
    testObj->run();
}

TEST_F(TestModbusSlaveControllerImpl, runFailedReceive)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_modbusSlaveMock, receive(_))
      .WillOnce(DoAll(testing::SetArgReferee<0>(m_modbusTcpRequest), Return(Gateway::ModbusReceiveStatus::FAILED)));
    EXPECT_CALL(*m_timerMock, restart()).Times(1);
    EXPECT_CALL(*m_modbusRequestControllerMock, forwardModbusRequestAndWaitForResponse(_)).Times(0);
    EXPECT_CALL(*m_modbusSlaveMock, reply(_)).Times(0);
    testObj->run();
}

TEST_F(TestModbusSlaveControllerImpl, runWithUnsupportedFunctionCode)
{
    auto testObj = createTestObject();

    // set unsupported function code
    m_modbusTcpRequest->functionCode = 0x17;

    Sequence seq;
    EXPECT_CALL(*m_modbusSlaveMock, receive(_))
      .InSequence(seq)
      .WillOnce(DoAll(testing::SetArgReferee<0>(m_modbusTcpRequest), Return(Gateway::ModbusReceiveStatus::OK)));
    EXPECT_CALL(*m_timerMock, restart()).InSequence(seq);
    EXPECT_CALL(*m_modbusRequestControllerMock, forwardModbusRequestAndWaitForResponse(_)).Times(0);
    EXPECT_CALL(*m_modbusSlaveMock, reply(_)).Times(0);
    EXPECT_CALL(*m_modbusSlaveMock, replyException(+Entity::ModbusExceptionCode::ILLEGAL_FUNCTION))
      .InSequence(seq)
      .WillOnce(Return(Gateway::ModbusReceiveStatus::FAILED)); // break loop
    testObj->run();
}

TEST_F(TestModbusSlaveControllerImpl, runFailedReply)
{
    auto testObj = createTestObject();

    Sequence seq;
    EXPECT_CALL(*m_modbusSlaveMock, receive(_))
      .InSequence(seq)
      .WillOnce(DoAll(testing::SetArgReferee<0>(m_modbusTcpRequest), Return(Gateway::ModbusReceiveStatus::OK)));
    EXPECT_CALL(*m_timerMock, restart()).InSequence(seq);
    EXPECT_CALL(*m_modbusRequestControllerMock, forwardModbusRequestAndWaitForResponse(_))
      .InSequence(seq)
      .WillOnce(Return(std::make_shared<ModbusTcpResponse>(m_modbusTcpRequest, ModbusOperationStatus::SUCCESS)));
    EXPECT_CALL(*m_modbusSlaveMock, reply(_)).InSequence(seq).WillOnce(Return(Gateway::ModbusReceiveStatus::FAILED));
    testObj->run();
}

TEST_F(TestModbusSlaveControllerImpl, runTimeoutExceptionReply)
{
    auto testObj = createTestObject();

    Sequence seq;
    EXPECT_CALL(*m_modbusSlaveMock, receive(_))
      .InSequence(seq)
      .WillOnce(DoAll(testing::SetArgReferee<0>(m_modbusTcpRequest), Return(Gateway::ModbusReceiveStatus::OK)));
    EXPECT_CALL(*m_timerMock, restart()).InSequence(seq);
    EXPECT_CALL(*m_modbusRequestControllerMock, forwardModbusRequestAndWaitForResponse(_))
      .InSequence(seq)
      .WillOnce(Return(std::make_shared<ModbusTcpResponse>(m_modbusTcpRequest, ModbusOperationStatus::TIMEOUT)));
    EXPECT_CALL(*m_modbusSlaveMock, reply(_)).Times(0);
    EXPECT_CALL(*m_modbusSlaveMock, replyException(+ModbusExceptionCode::GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND))
      .InSequence(seq)
      .WillOnce(Return(Gateway::ModbusReceiveStatus::FAILED)); // break loop
    testObj->run();
}

TEST_F(TestModbusSlaveControllerImpl, closeConnection)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_modbusSlaveMock, close()).Times(1);
    testObj->disconnect();
}

TEST_F(TestModbusSlaveControllerImpl, isExternalMasterConnected_true)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_modbusSlaveMock, isConnectionUp()).WillOnce(Return(true));
    EXPECT_TRUE(testObj->isExternalMasterConnected());
}

TEST_F(TestModbusSlaveControllerImpl, isExternalMasterConnected_false)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_modbusSlaveMock, isConnectionUp()).WillOnce(Return(false));
    EXPECT_FALSE(testObj->isExternalMasterConnected());
}

}
