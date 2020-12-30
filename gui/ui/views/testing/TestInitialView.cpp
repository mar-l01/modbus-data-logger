#include "domain/application/testing/gmock/MockModbusDataLoggerFacade.hpp"
#include "ui/views/includes/InitialView.hpp"

#include "gtest/gtest.h"

#include <QSignalSpy>
#include <memory>

namespace {

using namespace Views;

class TestInitialView : public ::testing::Test
{
protected:
    TestInitialView()
        : m_mbDataLoggerFacadeMock(std::make_shared<MockModbusDataLoggerFacade>())
    {}

    std::shared_ptr<InitialView> createTestObject()
    {
        return std::make_shared<InitialView>(m_mbDataLoggerFacadeMock);
    }

    std::shared_ptr<MockModbusDataLoggerFacade> m_mbDataLoggerFacadeMock;
};

TEST_F(TestInitialView, ctorSuccessful)
{
    EXPECT_NO_THROW(createTestObject());
}

TEST_F(TestInitialView, startModbusApplication)
{
    auto testObj = createTestObject();
    QSignalSpy mbRunningSignalSpy(testObj.get(), &InitialView::mbAppRunningChanged);

    EXPECT_CALL(*m_mbDataLoggerFacadeMock, startModbusCommunication());
    testObj->startModbusApplication();
    EXPECT_EQ(mbRunningSignalSpy.count(), 1); // make sure signal was emitted exactly one time
}

TEST_F(TestInitialView, stopModbusApplication)
{
    auto testObj = createTestObject();
    QSignalSpy mbRunningSignalSpy(testObj.get(), &InitialView::mbAppRunningChanged);

    EXPECT_CALL(*m_mbDataLoggerFacadeMock, stopModbusCommunication());
    testObj->stopModbusApplication();
    EXPECT_EQ(mbRunningSignalSpy.count(), 1); // make sure signal was emitted exactly one time
}

}
