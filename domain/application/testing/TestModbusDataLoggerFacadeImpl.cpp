#include "domain/application/includes/ModbusDataLoggerFacadeImpl.hpp"
#include "domain/gateway/testing/gmock/MockModbusRequestController.hpp"
#include "domain/gateway/testing/gmock/MockModbusResponseController.hpp"
#include "domain/logging/testing/gmock/MockFileLogger.hpp"

#include "gtest/gtest.h"

namespace {

using namespace ::testing;
using namespace Application;
using namespace Gateway;
using namespace Logging;

class TestModbusDataLoggerFacadeImpl : public ::testing::Test
{
protected:
    TestModbusDataLoggerFacadeImpl()
        : m_modbusRequestControllerMock(std::make_shared<MockModbusRequestController>())
        , m_modbusResponseControllerMock(std::make_shared<MockModbusResponseController>())
        , m_fileLoggerMock(std::make_shared<MockFileLogger>())
    {}

    std::shared_ptr<ModbusDataLoggerFacadeImpl> createTestObject()
    {
        return std::make_shared<ModbusDataLoggerFacadeImpl>(m_modbusRequestControllerMock,
                                                            m_modbusResponseControllerMock, m_fileLoggerMock);
    }

    std::shared_ptr<MockModbusRequestController> m_modbusRequestControllerMock;
    std::shared_ptr<MockModbusResponseController> m_modbusResponseControllerMock;
    std::shared_ptr<MockFileLogger> m_fileLoggerMock;
};

TEST_F(TestModbusDataLoggerFacadeImpl, ctorSuccessful)
{
    EXPECT_NO_THROW(createTestObject());
}

TEST_F(TestModbusDataLoggerFacadeImpl, startModbusCommunication)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_modbusRequestControllerMock, connect()).Times(1);
    testObj->startModbusCommunication();
}

TEST_F(TestModbusDataLoggerFacadeImpl, stopModbusCommunication)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_modbusRequestControllerMock, closeConnection()).Times(1);
    EXPECT_CALL(*m_modbusResponseControllerMock, closeConnection()).Times(1);
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
