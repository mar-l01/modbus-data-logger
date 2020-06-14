#pragma once

#include "domain/utility/interfaces/Timer.hpp"

#include <atomic>

namespace Utility {

class TimerImpl : public Timer
{
public:
    TimerImpl();

    void callOnTimeout(const int timeout, const std::function<void()>& callback) override;

private:
    std::atomic_bool m_isRunning;
};

}
