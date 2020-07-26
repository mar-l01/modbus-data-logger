#pragma once

#include "domain/utility/interfaces/Timer.hpp"

#include <memory>

namespace Utility {

class TimerFactory
{
public:
    static std::shared_ptr<Timer> createLoopTimer(const int frequencyInMs);
};

}
