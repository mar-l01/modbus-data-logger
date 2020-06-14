#include "domain/utility/includes/TimerImpl.hpp"

#include <chrono>
#include <iostream>
#include <thread>

namespace Utility {

TimerImpl::TimerImpl()
    : m_isRunning(false)
{}

void TimerImpl::callOnTimeout(const int timeoutInMs, const std::function<void()>& callback)
{
    if (m_isRunning) {
        std::cerr << "[Timer] Registering callback not possible because timer is currently running\n";
        return;
    }

    m_isRunning = true;

    // start extra thread to wait until timeout is reached
    std::thread waitThread([this, timeoutInMs, callback]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeoutInMs));
        callback();
        m_isRunning = false;
    });
    waitThread.detach();
}

}
