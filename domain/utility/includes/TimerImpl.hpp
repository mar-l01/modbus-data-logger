#pragma once

#include "domain/utility/interfaces/Timer.hpp"

#include <atomic>

namespace Utility {

/**
 * @brief This class represents a timer, which is used to fire a timeout once the corresponding time has expired.
 * It implemented the @ref Timer interface.
 */
class TimerImpl : public Timer
{
public:
    /**
     * @brief Construct a new TimerImpl object.
     */
    TimerImpl();

    /**
     * @see Timer::setTimeoutInMs
     */
    void setTimeoutInMs(const unsigned int timeoutInMs) override;

    /**
     * @see Timer::callOnTimeout
     */
    void callOnTimeout(const std::function<void()>& callback) override;

    /**
     * @see Timer::restart
     */
    void restart() override;

    /**
     * @see Timer::stop
     */
    void stop() override;

    /**
     * @brief Allow to set the loop frequency: The time which the loop sleeps after each iteration.
     *
     * @param frequencyInMs A frequency value provided in milliseconds.
     */
    void setFrequencyInMs(const int frequencyInMs);

private:
    std::atomic_bool m_isRunning;
    std::atomic_bool m_restartTimer;
    std::atomic_bool m_stopTimer;
    unsigned int m_timeoutInMs;
    int m_frequencyInMs;
};

}
