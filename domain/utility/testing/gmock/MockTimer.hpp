#pragma once

#include "domain/utility/interfaces/Timer.hpp"

#include "gmock/gmock.h"

class MockTimer : public Utility::Timer
{
public:
    MOCK_METHOD(void, setTimeoutInMs, (const unsigned int), (override));
    MOCK_METHOD(void, callOnTimeout, (const std::function<void()>&), (override));
    MOCK_METHOD(void, restart, (), (override));
    MOCK_METHOD(void, stop, (), (override));
};
