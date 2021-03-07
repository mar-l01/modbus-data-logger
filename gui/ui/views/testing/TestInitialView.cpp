#include "ui/facade/includes/ModbusDataLoggerSignals.hpp"
#include "ui/views/includes/InitialView.hpp"

#include "gtest/gtest.h"

#include <QSignalSpy>
#include <memory>

namespace {

using namespace Views;
using namespace Facade;

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

    std::shared_ptr<Facade::ModbusDataLoggerSignals> m_mbDataLoggerSignals;
};

TEST_F(TestInitialView, ctorSuccessful)
{
    EXPECT_NO_THROW(createTestObject());
}

TEST_F(TestInitialView, startModbusApplication)
{
    auto testObj = createTestObject();
    QSignalSpy mbApplicationStateChangesSignalSpy(m_mbDataLoggerSignals.get(),
                                                  &ModbusDataLoggerSignals::startListenForApplicationStateChanges);
    QSignalSpy mbStartCommunicationSignalSpy(m_mbDataLoggerSignals.get(),
                                             &ModbusDataLoggerSignals::startModbusCommunication);
    QSignalSpy mbRunningSignalSpy(testObj.get(), &InitialView::mbAppRunningChanged);

    testObj->startModbusApplication();

    // make sure signals were emitted exactly one time
    EXPECT_EQ(mbApplicationStateChangesSignalSpy.count(), 1);
    EXPECT_EQ(mbStartCommunicationSignalSpy.count(), 1);
    EXPECT_EQ(mbRunningSignalSpy.count(), 1);
}

TEST_F(TestInitialView, stopModbusApplication)
{
    auto testObj = createTestObject();
    QSignalSpy mbStopCommunicationSignalSpy(m_mbDataLoggerSignals.get(),
                                            &ModbusDataLoggerSignals::stopModbusCommunication);
    QSignalSpy mbApplicationStateChangesSignalSpy(m_mbDataLoggerSignals.get(),
                                                  &ModbusDataLoggerSignals::stopListenForApplicationStateChanges);
    QSignalSpy mbRunningSignalSpy(testObj.get(), &InitialView::mbAppRunningChanged);

    testObj->stopModbusApplication();

    // make sure signals were emitted exactly one time
    EXPECT_EQ(mbStopCommunicationSignalSpy.count(), 1);
    EXPECT_EQ(mbApplicationStateChangesSignalSpy.count(), 1);
    EXPECT_EQ(mbRunningSignalSpy.count(), 1);
}

}
