#include "domain/application/testing/gmock/MockModbusDataLoggerFacade.hpp"
#include "ui/views/includes/InitialView.hpp"

#include "gtest/gtest.h"

#include <memory>

namespace {

using namespace Views;

class TestInitialView : public ::testing::Test
{
protected:
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

}
