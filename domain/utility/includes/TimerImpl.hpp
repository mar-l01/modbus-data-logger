#pragma once

#include "domain/utility/interfaces/Timer.hpp"

namespace Utility {

class TimerImpl : public Timer
{
public:
    TimerImpl();

    void setTimeout(const int timeout, const std::function<void()>& callback) override;

private:
    std::function<void()> m_timeoutCallback;
};

}
