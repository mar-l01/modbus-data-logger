#include "domain/application/includes/ModbusDataLoggerFacadeImpl.hpp"
#include "domain/gateway/testing/gmock/MockModbusMasterController.hpp"
#include "domain/gateway/testing/gmock/MockModbusSlaveController.hpp"
#include "domain/logging/testing/gmock/MockFileLogger.hpp"
#include "domain/utility/testing/gmock/MockTimer.hpp"

#include "gmock/gmock.h"
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
    std::function<void()> timeoutCallback; // capture callback to invoke it later

    EXPECT_CALL(*m_modbusMasterControllerMock, connect()).Times(1);
    EXPECT_CALL(*m_modbusSlaveControllerMock, waitForIncomingConnection()).Times(AnyNumber());
    EXPECT_CALL(*m_modbusSlaveControllerMock, run()).Times(AnyNumber());
    EXPECT_CALL(*m_timerMock, callOnTimeout(_)).WillOnce(SaveArg<0>(&timeoutCallback));
    testObj->startModbusCommunication();

    // let's wait a bit until the Modbus loop is RUNNING to not invoke following function before
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // nothing will happen if started twice
    testObj->startModbusCommunication();

    // invoke callback to simulate timeout and stop created thread
    EXPECT_CALL(*m_modbusSlaveControllerMock, disconnect()).Times(1);
    EXPECT_CALL(*m_modbusMasterControllerMock, disconnect()).Times(1);
    timeoutCallback();
}

TEST_F(TestModbusDataLoggerFacadeImpl, stopModbusCommunication)
{
    auto testObj = createTestObject();

    // nothing will happen if stop is called if application was never started
    testObj->stopModbusCommunication();

    // pre-condition: application has to be started
    EXPECT_CALL(*m_modbusMasterControllerMock, connect()).Times(1);
    EXPECT_CALL(*m_modbusSlaveControllerMock, waitForIncomingConnection()).Times(AnyNumber());
    EXPECT_CALL(*m_modbusSlaveControllerMock, run()).Times(AnyNumber());
    EXPECT_CALL(*m_timerMock, callOnTimeout(_)).Times(1);
    testObj->startModbusCommunication();

    // let's wait a bit until the Modbus loop is RUNNING to not invoke following function before
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_CALL(*m_timerMock, stop()).Times(1);
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

TEST_F(TestModbusDataLoggerFacadeImpl, defaultApplicationState)
{
    auto testObj = createTestObject();

    EXPECT_EQ(ApplicationState::STOPPED, testObj->getCurrentApplicationState());
}

TEST_F(TestModbusDataLoggerFacadeImpl, applicationStateEvent)
{
    auto testObj = createTestObject();
    std::function<void()> timeoutCallback; // capture callback to invoke it later

    MockFunction<void(const ApplicationState&)> signalCallback;
    auto connectionPointer = testObj->addApplicationStateListener(signalCallback.AsStdFunction());

    {
        InSequence inSeq;
        EXPECT_CALL(signalCallback, Call(ApplicationState::STARTING)).Times(1);
        EXPECT_CALL(*m_modbusMasterControllerMock, connect()).Times(1);
        EXPECT_CALL(signalCallback, Call(ApplicationState::STARTED)).Times(1);
        EXPECT_CALL(*m_timerMock, callOnTimeout(_)).WillOnce(SaveArg<0>(&timeoutCallback));
        EXPECT_CALL(signalCallback, Call(ApplicationState::RUNNING)).Times(1);
    }

    // infinite for-loop calls (until callback is triggered below)
    EXPECT_CALL(*m_modbusSlaveControllerMock, waitForIncomingConnection()).Times(AnyNumber());
    EXPECT_CALL(*m_modbusSlaveControllerMock, run()).Times(AnyNumber());
    testObj->startModbusCommunication();

    // let's wait a bit until the Modbus loop is RUNNING to not invoke timeout before
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    {
        InSequence inSeq;
        EXPECT_CALL(signalCallback, Call(ApplicationState::STOPPING)).Times(1);
        EXPECT_CALL(*m_modbusSlaveControllerMock, disconnect()).Times(1);
        EXPECT_CALL(*m_modbusMasterControllerMock, disconnect()).Times(1);
        EXPECT_CALL(signalCallback, Call(ApplicationState::STOPPED)).Times(1);
    }
    // invoke callback to simulate timeout and stop created thread
    timeoutCallback();
}

}
