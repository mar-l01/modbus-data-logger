#pragma once

#include "domain/utility/interfaces/Timer.hpp"

#include <atomic>

namespace Utility {

class TimerImpl : public Timer
{
public:
    TimerImpl();

    void callOnTimeout(const int timeoutInMs, const std::function<void()>& callback) override;
    void restart() override;
    void stop() override;

    void setFrequencyInMs(const int frequencyInMs);

private:
    std::atomic_bool m_isRunning;
    std::atomic_bool m_restartTimer;
    std::atomic_bool m_stopTimer;
    int m_frequencyInMs;
};

}
