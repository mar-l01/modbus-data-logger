#pragma once

#include <functional>

namespace Utility {

/**
 * @brief This interface provides methods to be used when implementing a timer, which fires on timeout.
 */
class Timer
{
public:
    /**
     * @brief Once given timeout occurred, the provided callback function will be invoked.
     *
     * @param timeoutInMs Timeout value which triggers the callback.
     * @param callback Callback function which gets invoked once the time has expired.
     */
    virtual void callOnTimeout(const int timeoutInMs, const std::function<void()>& callback) = 0;

    /**
     * @brief Restart the timer.
     */
    virtual void restart() = 0;

    /**
     * @brief Stop the timer.
     */
    virtual void stop() = 0;
};

}
