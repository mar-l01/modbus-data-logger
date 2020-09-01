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
    TestTimer()
        : m_timeoutInMs(50)
        , m_timeoutOffset(20)
    {}

    std::shared_ptr<Timer> createTestObject()
    {
        auto testObj = std::make_shared<TimerImpl>();

        // set timeout on construction
        testObj->setTimeoutInMs(m_timeoutInMs);

        return testObj;
    }

    int m_timeoutInMs;
    int m_timeoutOffset;
};

TEST_F(TestTimer, callbackInvokedSuccessfully)
{
    auto testObj = createTestObject();

    MockFunction<void()> timeoutCallback;

    EXPECT_CALL(timeoutCallback, Call()).Times(1);
    testObj->callOnTimeout(timeoutCallback.AsStdFunction());

    // wait >'timeoutInMs' to make sure callback gets invoked
    std::this_thread::sleep_for(std::chrono::milliseconds(m_timeoutInMs + m_timeoutOffset));
}

TEST_F(TestTimer, registerOnlyOneTimeoutCallbackAtOnce)
{
    auto testObj = createTestObject();

    MockFunction<void()> timeoutCallback;
    MockFunction<void()> timeoutCallbackNotInvoked;

    EXPECT_CALL(timeoutCallback, Call()).Times(1);
    EXPECT_CALL(timeoutCallbackNotInvoked, Call()).Times(0);
    testObj->callOnTimeout(timeoutCallback.AsStdFunction());
    testObj->callOnTimeout(timeoutCallbackNotInvoked.AsStdFunction());

    // wait >'timeoutInMs' to make sure callback gets invoked
    std::this_thread::sleep_for(std::chrono::milliseconds(m_timeoutInMs + m_timeoutOffset));
}

TEST_F(TestTimer, checkCorrectResetOfRunningFlag)
{
    auto testObj = createTestObject();

    MockFunction<void()> timeoutCallback;

    EXPECT_CALL(timeoutCallback, Call()).Times(1);
    testObj->callOnTimeout(timeoutCallback.AsStdFunction());

    // wait >'timeoutInMs' to make sure callback gets invoked
    std::this_thread::sleep_for(std::chrono::milliseconds(m_timeoutInMs + m_timeoutOffset));

    // running-flag should have been reset here, another timeout can be registered
    EXPECT_CALL(timeoutCallback, Call()).Times(1);
    testObj->callOnTimeout(timeoutCallback.AsStdFunction());

    // wait >'timeoutInMs' to make sure callback gets invoked
    std::this_thread::sleep_for(std::chrono::milliseconds(m_timeoutInMs + m_timeoutOffset));
}

TEST_F(TestTimer, restartTimer)
{
    auto testObj = createTestObject();

    MockFunction<void()> timeoutCallback;

    // since restarting it below, callback should not be invoked here
    EXPECT_CALL(timeoutCallback, Call()).Times(0);
    testObj->callOnTimeout(timeoutCallback.AsStdFunction());

    // restart timer
    EXPECT_CALL(timeoutCallback, Call()).Times(1);
    testObj->restart();

    // wait >'timeoutInMs' to make sure callback gets invoked
    std::this_thread::sleep_for(std::chrono::milliseconds(m_timeoutInMs + m_timeoutOffset));
}

TEST_F(TestTimer, stopTimer)
{
    auto testObj = createTestObject();

    MockFunction<void()> timeoutCallback;

    // since stopping it below, callback should not be invoked
    EXPECT_CALL(timeoutCallback, Call()).Times(0);
    testObj->callOnTimeout(timeoutCallback.AsStdFunction());

    EXPECT_CALL(timeoutCallback, Call()).Times(0);
    testObj->stop();

    // wait >'timeoutInMs' to make sure callback doesn't get invoked
    std::this_thread::sleep_for(std::chrono::milliseconds(m_timeoutInMs + m_timeoutOffset));
}

TEST_F(TestTimer, restartAndStopTimer)
{
    auto testObj = createTestObject();

    MockFunction<void()> timeoutCallback;

    // since restarting it below, callback should not be invoked here
    EXPECT_CALL(timeoutCallback, Call()).Times(0);
    testObj->callOnTimeout(timeoutCallback.AsStdFunction());

    // restart timer, callback not invoked since timer is stopped below
    EXPECT_CALL(timeoutCallback, Call()).Times(0);
    testObj->restart();

    // stop timer
    EXPECT_CALL(timeoutCallback, Call()).Times(0);
    testObj->stop();

    // wait >'timeoutInMs' to make sure callback gets invoked
    std::this_thread::sleep_for(std::chrono::milliseconds(m_timeoutInMs + m_timeoutOffset));
}

TEST_F(TestTimer, stopAndRestartTimerNotPossible)
{
    auto testObj = createTestObject();

    MockFunction<void()> timeoutCallback;

    // since restarting it below, callback should not be invoked here
    EXPECT_CALL(timeoutCallback, Call()).Times(0);
    testObj->callOnTimeout(timeoutCallback.AsStdFunction());

    // stop timer
    EXPECT_CALL(timeoutCallback, Call()).Times(0);
    testObj->stop();

    // restart timer not possible
    EXPECT_CALL(timeoutCallback, Call()).Times(0);
    testObj->restart();

    // wait >'timeoutInMs' to make sure callback gets invoked
    std::this_thread::sleep_for(std::chrono::milliseconds(m_timeoutInMs + m_timeoutOffset));
}

}
