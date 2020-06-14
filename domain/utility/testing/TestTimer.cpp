#include "domain/utility/includes/TimerImpl.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <chrono>
#include <thread>

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
    int timeoutInMs = 100;

    EXPECT_CALL(timeoutCallback, Call()).Times(1);
    testObj->callOnTimeout(timeoutInMs, timeoutCallback.AsStdFunction());

    // wait >'timeoutInMs' to make sure callback gets invoked
    std::this_thread::sleep_for(std::chrono::milliseconds(timeoutInMs + 50));
}

TEST_F(TestTimer, registerOnlyOneTimeoutCallbackAtOnce)
{
    auto testObj = createTestObject();

    MockFunction<void()> timeoutCallback;
    MockFunction<void()> timeoutCallbackNotInvoked;
    int timeoutInMs = 100;

    EXPECT_CALL(timeoutCallback, Call()).Times(1);
    EXPECT_CALL(timeoutCallbackNotInvoked, Call()).Times(0);
    testObj->callOnTimeout(timeoutInMs, timeoutCallback.AsStdFunction());
    testObj->callOnTimeout(timeoutInMs, timeoutCallbackNotInvoked.AsStdFunction());

    // wait >'timeoutInMs' to make sure callback gets invoked
    std::this_thread::sleep_for(std::chrono::milliseconds(timeoutInMs + 50));
}

TEST_F(TestTimer, checkCorrectResetOfRunningFlag)
{
    auto testObj = createTestObject();

    MockFunction<void()> timeoutCallback;
    int timeoutInMs = 100;

    EXPECT_CALL(timeoutCallback, Call()).Times(1);
    testObj->callOnTimeout(timeoutInMs, timeoutCallback.AsStdFunction());

    // wait >'timeoutInMs' to make sure callback gets invoked
    std::this_thread::sleep_for(std::chrono::milliseconds(timeoutInMs + 50));

    // running-flag should have been reset here, another timeout can be registered
    EXPECT_CALL(timeoutCallback, Call()).Times(1);
    testObj->callOnTimeout(timeoutInMs, timeoutCallback.AsStdFunction());

    // wait >'timeoutInMs' to make sure callback gets invoked
    std::this_thread::sleep_for(std::chrono::milliseconds(timeoutInMs + 50));
}

}
