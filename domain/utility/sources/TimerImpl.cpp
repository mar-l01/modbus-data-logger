#include "domain/utility/includes/TimerImpl.hpp"

#include <chrono>
#include <iostream>
#include <thread>

namespace Utility {

TimerImpl::TimerImpl()
    : m_isRunning(false)
    , m_restartTimer(false)
{}

void TimerImpl::callOnTimeout(const int timeoutInMs, const std::function<void()>& callback)
{
    if (m_isRunning) {
        std::cerr << "[Timer] Registering callback not possible because timer is currently running\n";
        return;
    }

    m_isRunning = true;
    m_restartTimer = false;

    auto startTime = std::chrono::steady_clock::now();

    // start extra thread to wait until timeout is reached
    std::thread waitThread([this, timeoutInMs, callback]() {
        auto startTime = std::chrono::steady_clock::now();

        for (;;) {
            // sleep 1 ms and check again
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            if (m_restartTimer) {
                // restart timer if required
                startTime = std::chrono::steady_clock::now();
                m_restartTimer = false;
            } else if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() -
                                                                             startTime)
                         .count() >= timeoutInMs) {
                // stop and invoke callback if timeout is reached
                break;
            }
        }
        callback();
        m_isRunning = false;
    });
    waitThread.detach();
}

void TimerImpl::restart()
{
    m_restartTimer = true;
}

}
