#pragma once

#include "domain/utility/interfaces/Timer.hpp"

#include <memory>

namespace Utility {

/**
 * @brief Factory to create a @ref Timer instance.
 */
class TimerFactory
{
public:
    /**
     * @brief Create a LoopTimer object.
     *
     * @param frequencyInMs The frequency in which the loop will sleep, provided in milliseconds.
     * @return A shared-ptr instance of type @ref Timer.
     */
    static std::shared_ptr<Timer> createLoopTimer(const int frequencyInMs = 1);
};

}
