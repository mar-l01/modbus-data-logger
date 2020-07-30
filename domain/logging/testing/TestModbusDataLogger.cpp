#include "domain/logging/includes/ModbusDataLogger.hpp"
#include "domain/logging/testing/gmock/MockRealTimeLogger.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using namespace ::testing;
using namespace Logging;

class TestModbusDataLogger : public ::testing::Test
{
protected:
    std::shared_ptr<ModbusDataLogger> createTestObject()
    {
        auto testObj = std::make_shared<ModbusDataLogger>();

        return testObj;
    }

    std::shared_ptr<MockRealTimeLogger> m_realTimeLoggerMock;
};

TEST_F(TestModbusDataLogger, ctorSuccessful)
{
    EXPECT_NO_THROW(createTestObject());
}

}
