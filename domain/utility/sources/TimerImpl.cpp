#include "domain/utility/includes/TimerImpl.hpp"

#include <chrono>

namespace Utility {

TimerImpl::TimerImpl() {}

void TimerImpl::setTimeout(const int timeout, const std::function<void()>& callback)
{
    m_timeoutCallback = callback;
}

}
