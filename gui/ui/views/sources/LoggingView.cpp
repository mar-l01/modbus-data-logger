#include "ui/views/includes/LoggingView.hpp"

#include "ui/facade/includes/ModbusDataLoggerSignals.hpp"

namespace Views {

using Application::ApplicationState;

LoggingView::LoggingView(const std::shared_ptr<Facade::ModbusDataLoggerSignals>& mbDataLoggerSignals)
    : m_mbDataLoggerSignals(mbDataLoggerSignals)
{
    connect(m_mbDataLoggerSignals.get(), &Facade::ModbusDataLoggerSignals::applicationStateChanged, this,
            &LoggingView::onApplicationStateChanged);

    connect(m_mbDataLoggerSignals.get(), &Facade::ModbusDataLoggerSignals::startLogger, this,
            &LoggingView::onLoggingStarted);
    connect(m_mbDataLoggerSignals.get(), &Facade::ModbusDataLoggerSignals::stopLogger, this,
            &LoggingView::onLoggingStopped);
}

void LoggingView::onApplicationStateChanged(ApplicationState applicationState)
{
    switch (applicationState) {
        case ApplicationState::STARTING:
            break;

        case ApplicationState::STARTED:
            break;

        case ApplicationState::RUNNING:
            break;

        case ApplicationState::STOPPING:
            break;

        case ApplicationState::STOPPED:
            break;
    }
}

void LoggingView::onLoggingStarted() {}

void LoggingView::onLoggingStopped() {}

}
