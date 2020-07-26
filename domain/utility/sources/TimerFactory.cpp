#include "domain/utility/includes/TimerFactory.hpp"

#include "domain/utility/includes/TimerImpl.hpp"

namespace Utility {

std::shared_ptr<Timer> TimerFactory::createLoopTimer(const int frequencyInMs)
{
    return std::make_shared<TimerImpl>();
}

}
