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
     * @brief Set the timeout on which a callback should be invoked (if given)
     *
     * @param timeoutInMs Timeout value which triggers the callback.
     */
    virtual void setTimeoutInMs(const unsigned int timeoutInMs) = 0;

    /**
     * @brief Once the timeout occurred, the provided callback function will be invoked.
     *
     * @param callback Callback function which gets invoked once the time has expired.
     * @ref setTimeoutInMs
     */
    virtual void callOnTimeout(const std::function<void()>& callback) = 0;

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
