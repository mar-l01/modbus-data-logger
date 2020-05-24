#include "domain/gateway/includes/ModbusMasterController.hpp"
#include "domain/gateway/testing/gmock/MockModbusMaster.hpp"
#include "domain/gateway/testing/gmock/MockModbusResponseController.hpp"

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
        , m_modbusResponseControllerMock(std::make_shared<MockModbusResponseController>())
        , m_ipAddr("127.0.0.1")
        , m_port(502)
    {}

    std::shared_ptr<ModbusMasterController> createTestObject()
    {
        auto testObj = std::make_shared<ModbusMasterController>(m_modbusMasterMock, m_ipAddr, m_port);

        return testObj;
    }

    std::shared_ptr<MockModbusMaster> m_modbusMasterMock;
    std::shared_ptr<MockModbusResponseController> m_modbusResponseControllerMock;
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

TEST_F(TestModbusMasterController, closeConnection)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_modbusMasterMock, close()).Times(1);
    testObj->closeConnection();
}

}
