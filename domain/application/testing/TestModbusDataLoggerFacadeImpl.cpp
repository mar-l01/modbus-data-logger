#include "domain/application/includes/ModbusDataLoggerFacadeImpl.hpp"
#include "domain/gateway/testing/gmock/MockModbusMasterController.hpp"
#include "domain/gateway/testing/gmock/MockModbusSlaveController.hpp"
#include "domain/logging/testing/gmock/MockFileLogger.hpp"
#include "domain/utility/testing/gmock/MockTimer.hpp"

#include "gtest/gtest.h"

namespace {

using namespace ::testing;
using namespace Application;
using namespace Gateway;
using namespace Logging;
using namespace Utility;

class TestModbusDataLoggerFacadeImpl : public ::testing::Test
{
protected:
    TestModbusDataLoggerFacadeImpl()
        : m_modbusMasterControllerMock(std::make_shared<MockModbusMasterController>())
        , m_modbusSlaveControllerMock(std::make_shared<MockModbusSlaveController>())
        , m_fileLoggerMock(std::make_shared<MockFileLogger>())
        , m_timerMock(std::make_shared<MockTimer>())
    {}

    std::shared_ptr<ModbusDataLoggerFacadeImpl> createTestObject()
    {
        return std::make_shared<ModbusDataLoggerFacadeImpl>(m_modbusMasterControllerMock, m_modbusSlaveControllerMock,
                                                            m_fileLoggerMock, m_timerMock);
    }

    std::shared_ptr<MockModbusMasterController> m_modbusMasterControllerMock;
    std::shared_ptr<MockModbusSlaveController> m_modbusSlaveControllerMock;
    std::shared_ptr<MockFileLogger> m_fileLoggerMock;
    std::shared_ptr<MockTimer> m_timerMock;
};

TEST_F(TestModbusDataLoggerFacadeImpl, ctorSuccessful)
{
    EXPECT_NO_THROW(createTestObject());
}

TEST_F(TestModbusDataLoggerFacadeImpl, startModbusCommunication)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_modbusMasterControllerMock, connect()).Times(1);
    EXPECT_CALL(*m_modbusSlaveControllerMock, waitForIncomingConnection()).Times(1);
    testObj->startModbusCommunication();
}

TEST_F(TestModbusDataLoggerFacadeImpl, stopModbusCommunication)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_modbusSlaveControllerMock, disconnect()).Times(1);
    EXPECT_CALL(*m_modbusMasterControllerMock, disconnect()).Times(1);
    testObj->stopModbusCommunication();
}

TEST_F(TestModbusDataLoggerFacadeImpl, startLogger)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_fileLoggerMock, startLogging()).Times(1);
    testObj->startLogger();
}

TEST_F(TestModbusDataLoggerFacadeImpl, stopLogger)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_fileLoggerMock, stopLogging()).Times(1);
    testObj->stopLogger();
}

}
