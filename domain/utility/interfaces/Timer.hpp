#pragma once

#include <functional>

namespace Utility {

class Timer
{
public:
    virtual void callOnTimeout(const int timeoutInMs, const std::function<void()>& callback) = 0;
    virtual void restart() = 0;
};

}
