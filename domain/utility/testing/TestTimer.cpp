#include "domain/utility/includes/TimerImpl.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using namespace ::testing;
using namespace Utility;

class TestTimer : public ::testing::Test
{
protected:
    std::shared_ptr<Timer> createTestObject()
    {
        auto testObj = std::make_shared<TimerImpl>();

        return testObj;
    }
};

TEST_F(TestTimer, callbackInvokedSuccessfully)
{
    auto testObj = createTestObject();

    MockFunction<void()> timeoutCallback;
    int timeout = 500; // in ms

    EXPECT_CALL(timeoutCallback, Call()).Times(1);
    testObj->callOnTimeout(timeout, timeoutCallback.AsStdFunction());
}

}
