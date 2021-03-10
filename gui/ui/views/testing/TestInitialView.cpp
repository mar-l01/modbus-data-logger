#include "domain/framework/includes/SignalEvent.hpp"
#include "ui/facade/includes/ModbusDataLoggerSignals.hpp"
#include "ui/views/includes/InitialView.hpp"

#include "gtest/gtest.h"

#include <QSignalSpy>
#include <memory>

namespace {

using namespace Views;
using namespace Facade;
using Application::ApplicationState;
using Framework::SignalEvent;

class TestInitialView : public ::testing::Test
{
protected:
    TestInitialView()
        : m_mbDataLoggerSignals(std::make_shared<ModbusDataLoggerSignals>())
    {}

    std::shared_ptr<InitialView> createTestObject()
    {
        return std::make_shared<InitialView>(m_mbDataLoggerSignals);
    }

    std::shared_ptr<ModbusDataLoggerSignals> m_mbDataLoggerSignals;
};

TEST_F(TestInitialView, ctorSuccessful)
{
    EXPECT_NO_THROW(createTestObject());
}

TEST_F(TestInitialView, startModbusApplication)
{
    auto testObj = createTestObject();
    QSignalSpy mbApplicationStateChangedSignalSpy(m_mbDataLoggerSignals.get(),
                                                  &ModbusDataLoggerSignals::startListenForApplicationStateChanges);
    QSignalSpy mbStartCommunicationSignalSpy(m_mbDataLoggerSignals.get(),
                                             &ModbusDataLoggerSignals::startModbusCommunication);
    QSignalSpy mbRunningSignalSpy(testObj.get(), &InitialView::mbAppRunningChanged);

    testObj->startModbusApplication();

    // make sure signals were emitted exactly one time
    EXPECT_EQ(mbApplicationStateChangedSignalSpy.count(), 1);
    EXPECT_EQ(mbStartCommunicationSignalSpy.count(), 1);
    EXPECT_EQ(mbRunningSignalSpy.count(), 1);
}

TEST_F(TestInitialView, stopModbusApplication)
{
    auto testObj = createTestObject();
    QSignalSpy mbStopCommunicationSignalSpy(m_mbDataLoggerSignals.get(),
                                            &ModbusDataLoggerSignals::stopModbusCommunication);
    QSignalSpy mbApplicationStateChangedSignalSpy(m_mbDataLoggerSignals.get(),
                                                  &ModbusDataLoggerSignals::stopListenForApplicationStateChanges);
    QSignalSpy mbRunningSignalSpy(testObj.get(), &InitialView::mbAppRunningChanged);

    testObj->stopModbusApplication();

    // make sure signals were emitted exactly one time
    EXPECT_EQ(mbStopCommunicationSignalSpy.count(), 1);
    EXPECT_EQ(mbApplicationStateChangedSignalSpy.count(), 1);
    EXPECT_EQ(mbRunningSignalSpy.count(), 1);
}

TEST_F(TestInitialView, buttonOccurrenceChange)
{
    // register domain scoped-enum to make it available in signal slot connections
    qRegisterMetaType<Application::ApplicationState>("Application::ApplicationState");

    auto testObj = createTestObject();
    QSignalSpy buttonEnabledChangedSignalSpy(testObj.get(), &InitialView::buttonEnabledChanged);
    QSignalSpy startButtonVisibilityChangedSignalSpy(testObj.get(), &InitialView::startButtonVisibilityChanged);

    // ----- STARTING -----
    emit m_mbDataLoggerSignals->applicationStateChanged(ApplicationState::STARTING);
    EXPECT_EQ(buttonEnabledChangedSignalSpy.count(), 1);
    EXPECT_EQ(startButtonVisibilityChangedSignalSpy.count(), 0);

    // ----- STARTED -----
    emit m_mbDataLoggerSignals->applicationStateChanged(ApplicationState::STARTED);
    EXPECT_EQ(buttonEnabledChangedSignalSpy.count(), 1);
    EXPECT_EQ(startButtonVisibilityChangedSignalSpy.count(), 1);

    // ----- RUNNING -----
    emit m_mbDataLoggerSignals->applicationStateChanged(ApplicationState::RUNNING);
    EXPECT_EQ(buttonEnabledChangedSignalSpy.count(), 2);
    EXPECT_EQ(startButtonVisibilityChangedSignalSpy.count(), 1);

    // ----- STOPPING -----
    emit m_mbDataLoggerSignals->applicationStateChanged(ApplicationState::STOPPING);
    EXPECT_EQ(buttonEnabledChangedSignalSpy.count(), 3);
    EXPECT_EQ(startButtonVisibilityChangedSignalSpy.count(), 1);

    // ----- STOPPED -----
    emit m_mbDataLoggerSignals->applicationStateChanged(ApplicationState::STOPPED);
    EXPECT_EQ(buttonEnabledChangedSignalSpy.count(), 4);
    EXPECT_EQ(startButtonVisibilityChangedSignalSpy.count(), 2);
}

}
