#include "domain/utility/includes/TimerImpl.hpp"

#include "spdlog/spdlog.h"
#include <chrono>
#include <thread>

namespace Utility {

TimerImpl::TimerImpl()
    : m_isRunning(false)
    , m_restartTimer(false)
    , m_stopTimer(false)
    , m_frequencyInMs(1) // default initialization to 1 ms
{}

void TimerImpl::callOnTimeout(const int timeoutInMs, const std::function<void()>& callback)
{
    if (m_isRunning) {
        SPDLOG_ERROR("Registering callback not possible because timer is currently running");
        return;
    }

    // init flags
    m_isRunning = true;
    m_restartTimer = false;
    m_stopTimer = false;

    // start extra thread to wait until timeout is reached
    std::thread waitThread([this, timeoutInMs, callback]() {
        auto startTime = std::chrono::steady_clock::now();

        for (;;) {
            // sleep 1 ms and check again
            std::this_thread::sleep_for(std::chrono::milliseconds(m_frequencyInMs));

            if (m_restartTimer) {
                // restart timer if required
                startTime = std::chrono::steady_clock::now();
                m_restartTimer = false;
            } else if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() -
                                                                             startTime)
                           .count() >= timeoutInMs ||
                       m_stopTimer) {
                // stop if timeout is reached or timer shall be stopped
                break;
            }
        }
        if (m_stopTimer) {
            m_stopTimer = false;
        } else {
            callback();
        }
        m_isRunning = false;
    });
    waitThread.detach();
}

void TimerImpl::restart()
{
    if (not m_stopTimer) {
        m_restartTimer = true;
    } else {
        SPDLOG_ERROR("Timer cannot be restarted because stopping it is in progress");
    }
}

void TimerImpl::stop()
{
    m_stopTimer = true;
}

void TimerImpl::setFrequencyInMs(const int frequencyInMs)
{
    m_frequencyInMs = frequencyInMs;
}

}
