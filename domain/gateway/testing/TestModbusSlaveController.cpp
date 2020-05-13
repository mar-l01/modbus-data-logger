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
        : m_modbusSlaveMock(std::make_unique<MockModbusSlave>())
        , m_modbusDataMapping({0, 0, 0, 0, 1, 2, 3, 4})
        , m_ipAddr("127.0.0.1")
        , m_port(502)
        , m_modbusSocket(-1)
    {}

    std::unique_ptr<ModbusSlaveController> createTestObject()
    {
        EXPECT_CALL(*m_modbusSlaveMock, setModbusDataMapping(_)).Times(1);
        auto testObj =
          std::make_unique<ModbusSlaveController>(std::move(m_modbusSlaveMock), m_modbusDataMapping, m_ipAddr, m_port);

        return std::move(testObj);
    }

    std::unique_ptr<MockModbusSlave> m_modbusSlaveMock;
    ModbusDataMapping m_modbusDataMapping;
    std::string m_ipAddr;
    int m_port;
    int m_modbusSocket;
};

TEST_F(TestModbusSlaveController, ctorSuccessful)
{
    EXPECT_NO_THROW(createTestObject());
}

TEST_F(TestModbusSlaveController, ctorModbusSlaveNullptr)
{
    EXPECT_ANY_THROW(ModbusSlaveController(nullptr, m_modbusDataMapping, m_ipAddr, m_port));
}

}
