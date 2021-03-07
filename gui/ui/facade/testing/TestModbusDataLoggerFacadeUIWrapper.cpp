#include "domain/application/testing/gmock/MockModbusDataLoggerFacade.hpp"
#include "domain/framework/includes/SignalEvent.hpp"
#include "ui/facade/includes/ModbusDataLoggerFacadeUIWrapper.hpp"
#include "ui/facade/includes/ModbusDataLoggerSignals.hpp"

#include "gtest/gtest.h"

#include <QSignalSpy>
#include <memory>

namespace {

using namespace ::testing;
using namespace Facade;
using Application::ApplicationState;
using Framework::ScopedConnection;
using Framework::SignalEvent;

class TestModbusDataLoggerFacadeUIWrapper : public Test
{
protected:
    TestModbusDataLoggerFacadeUIWrapper()
        : m_mbDataLoggerFacadeMock(std::make_shared<MockModbusDataLoggerFacade>())
        , m_mbDataLoggerSignals(std::make_shared<ModbusDataLoggerSignals>())
    {}

    std::shared_ptr<ModbusDataLoggerFacadeUIWrapper> createTestObject()
    {
        return std::make_shared<ModbusDataLoggerFacadeUIWrapper>(m_mbDataLoggerFacadeMock, m_mbDataLoggerSignals);
    }

    std::shared_ptr<MockModbusDataLoggerFacade> m_mbDataLoggerFacadeMock;
    std::shared_ptr<ModbusDataLoggerSignals> m_mbDataLoggerSignals;
    SignalEvent<ApplicationState> m_applicationStateEvent;
};

TEST_F(TestModbusDataLoggerFacadeUIWrapper, ctorSuccessful)
{
    EXPECT_NO_THROW(createTestObject());
}

TEST_F(TestModbusDataLoggerFacadeUIWrapper, onStartModbusCommunication)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_mbDataLoggerFacadeMock, startModbusCommunication());
    testObj->onStartModbusCommunication();
}

TEST_F(TestModbusDataLoggerFacadeUIWrapper, onStopModbusCommunication)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_mbDataLoggerFacadeMock, stopModbusCommunication());
    testObj->onStopModbusCommunication();
}

TEST_F(TestModbusDataLoggerFacadeUIWrapper, onStartLogger)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_mbDataLoggerFacadeMock, startLogger());
    testObj->onStartLogger();
}

TEST_F(TestModbusDataLoggerFacadeUIWrapper, onStopLogger)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_mbDataLoggerFacadeMock, stopLogger());
    testObj->onStopLogger();
}

TEST_F(TestModbusDataLoggerFacadeUIWrapper, onGetCurrentApplicationState)
{
    auto testObj = createTestObject();
    QSignalSpy mbApplicationStateChangedSignalSpy(m_mbDataLoggerSignals.get(),
                                                  &ModbusDataLoggerSignals::applicationStateChanged);
    auto expectedApplicationState = ApplicationState::RUNNING;

    EXPECT_CALL(*m_mbDataLoggerFacadeMock, getCurrentApplicationState()).WillOnce(Return(expectedApplicationState));
    testObj->onGetCurrentApplicationState();

    EXPECT_EQ(mbApplicationStateChangedSignalSpy.count(), 1);
    auto emittedApplicationState = mbApplicationStateChangedSignalSpy.takeFirst().at(0).toUInt();
    EXPECT_EQ(emittedApplicationState, static_cast<unsigned int>(expectedApplicationState));
}

TEST_F(TestModbusDataLoggerFacadeUIWrapper, onStartAndStopListenForApplicationStateChanges)
{
    auto testObj = createTestObject();
    QSignalSpy mbApplicationStateChangedSignalSpy(m_mbDataLoggerSignals.get(),
                                                  &ModbusDataLoggerSignals::applicationStateChanged);
    auto expectedApplicationState = ApplicationState::RUNNING;

    // ----- start listener -----
    EXPECT_CALL(*m_mbDataLoggerFacadeMock, addApplicationStateListener(_)).WillOnce(Invoke([this](auto signalCallback) {
        return std::make_shared<ScopedConnection>(m_applicationStateEvent.connect(signalCallback));
    }));
    testObj->onStartListenForApplicationStateChanges();

    // simulate application state change
    m_applicationStateEvent(expectedApplicationState);
    EXPECT_EQ(mbApplicationStateChangedSignalSpy.count(), 1);
    auto emittedApplicationState = mbApplicationStateChangedSignalSpy.takeFirst().at(0).toUInt();
    EXPECT_EQ(emittedApplicationState, static_cast<unsigned int>(expectedApplicationState));

    // ----- stop listener -----
    testObj->onStopListenForApplicationStateChanges();

    // application state change shouldn't be emitted
    m_applicationStateEvent(expectedApplicationState);
    EXPECT_EQ(mbApplicationStateChangedSignalSpy.count(), 0);
}

}
