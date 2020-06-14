#pragma once

#include "domain/utility/interfaces/Timer.hpp"

#include "gmock/gmock.h"

class MockTimer : public Utility::Timer
{
public:
    MOCK_METHOD(void, callOnTimeout, (const int, const std::function<void()>&), (override));
};
