#pragma once

#include <functional>

namespace Utility {

class Timer
{
public:
    virtual void callOnTimeout(const int timeout, const std::function<void()>& callback) = 0;
};

}
