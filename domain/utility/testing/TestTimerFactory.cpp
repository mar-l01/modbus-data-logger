#include "domain/utility/includes/TimerFactory.hpp"

#include "gtest/gtest.h"

namespace {

using namespace Utility;

TEST(TestTimerFactory, createLoopTimerReturnTimerInstance)
{
    auto timerInstance = TimerFactory::createLoopTimer();
    EXPECT_NE(timerInstance, nullptr);
}

}
