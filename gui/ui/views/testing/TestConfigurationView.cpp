#include "domain/framework/testing/gmock/MockConfigurationFileAccessor.hpp"
#include "ui/views/includes/ConfigurationView.hpp"

#include "gtest/gtest.h"

#include <memory>

namespace {

using namespace Framework;
using namespace Views;

class TestConfigurationView : public ::testing::Test
{
protected:
    TestConfigurationView()
        : m_configurationFileAccessorMock(std::make_shared<MockConfigurationFileAccessor>())
    {}

    std::shared_ptr<ConfigurationView> createTestObject()
    {
        return std::make_shared<ConfigurationView>(m_configurationFileAccessorMock);
    }

    std::shared_ptr<MockConfigurationFileAccessor> m_configurationFileAccessorMock;
};

TEST_F(TestConfigurationView, constructor)
{
    EXPECT_CALL(*m_configurationFileAccessorMock, getModbusConfiguration());
    EXPECT_NO_THROW(createTestObject());
}

}
