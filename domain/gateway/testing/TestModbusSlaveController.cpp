#include "domain/gateway/includes/ModbusSlaveController.hpp"
#include "domain/gateway/testing/gmock/MockModbusSlave.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using namespace ::testing;
using namespace Gateway;

class TestModbusSlaveController : public ::testing::Test
{
protected:
    TestModbusSlaveController()
        : m_modbusSlaveMock(std::make_shared<MockModbusSlave>())
        , m_modbusDataMapping({0, 0, 0, 0, 1, 2, 3, 4})
        , m_ipAddr("127.0.0.1")
        , m_port(502)
    {}

    std::shared_ptr<ModbusSlaveController> createTestObject()
    {
        EXPECT_CALL(*m_modbusSlaveMock, setModbusDataMapping(_)).Times(1);
        auto testObj =
          std::make_shared<ModbusSlaveController>(m_modbusSlaveMock, m_modbusDataMapping, m_ipAddr, m_port);

        return testObj;
    }

    std::shared_ptr<MockModbusSlave> m_modbusSlaveMock;
    ModbusDataMapping m_modbusDataMapping;
    std::string m_ipAddr;
    int m_port;
};

TEST_F(TestModbusSlaveController, ctorSuccessful)
{
    EXPECT_NO_THROW(createTestObject());
}

TEST_F(TestModbusSlaveController, connect)
{
    auto testObj = createTestObject();
    int modbusSocket = 1;

    EXPECT_CALL(*m_modbusSlaveMock, bind(m_ipAddr, m_port)).Times(1);
    EXPECT_CALL(*m_modbusSlaveMock, listen(1)).WillOnce(Return(modbusSocket));
    EXPECT_CALL(*m_modbusSlaveMock, accept(modbusSocket)).Times(1);
    testObj->connect();
}

TEST_F(TestModbusSlaveController, runOneFullLoop)
{
    auto testObj = createTestObject();

    Sequence seq;
    EXPECT_CALL(*m_modbusSlaveMock, receive(_)).InSequence(seq).WillOnce(Return(1));
    EXPECT_CALL(*m_modbusSlaveMock, reply(_)).InSequence(seq).WillOnce(Return(1));
    EXPECT_CALL(*m_modbusSlaveMock, receive(_)).InSequence(seq).WillOnce(Return(-1)); // break loop
    testObj->run();
}

TEST_F(TestModbusSlaveController, runReceiveLoop)
{
    auto testObj = createTestObject();

    Sequence seq;
    // 0 invokes the do-while loop again
    EXPECT_CALL(*m_modbusSlaveMock, receive(_)).InSequence(seq).WillOnce(Return(0));
    EXPECT_CALL(*m_modbusSlaveMock, receive(_)).InSequence(seq).WillOnce(Return(0));
    EXPECT_CALL(*m_modbusSlaveMock, receive(_)).InSequence(seq).WillOnce(Return(-1)); // break loop
    testObj->run();
}

TEST_F(TestModbusSlaveController, runFailedReceive)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_modbusSlaveMock, receive(_)).WillOnce(Return(-1));
    EXPECT_CALL(*m_modbusSlaveMock, reply(_)).Times(0);
    testObj->run();
}

TEST_F(TestModbusSlaveController, runFailedReply)
{
    auto testObj = createTestObject();

    Sequence seq;
    EXPECT_CALL(*m_modbusSlaveMock, receive(_)).InSequence(seq).WillOnce(Return(1));
    EXPECT_CALL(*m_modbusSlaveMock, reply(_)).InSequence(seq).WillOnce(Return(-1));
    testObj->run();
}

TEST_F(TestModbusSlaveController, closeConnection)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_modbusSlaveMock, close()).Times(1);
    testObj->closeConnection();
}

}