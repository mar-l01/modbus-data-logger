#include "ui/views/includes/InitialView.hpp"

#include "ui/facade/includes/ModbusDataLoggerSignals.hpp"

namespace Views {

using Application::ApplicationState;

InitialView::InitialView(const std::shared_ptr<Facade::ModbusDataLoggerSignals>& mbDataLoggerSignals)
    : m_mbDataLoggerSignals(mbDataLoggerSignals)
    , m_isMbAppRunning(false)
    , m_isButtonEnabled(true)
    , m_isStartButtonVisible(true)
{
    connect(m_mbDataLoggerSignals.get(), &Facade::ModbusDataLoggerSignals::applicationStateChanged, this,
            &InitialView::onApplicationStateChanged);
}

void InitialView::startModbusApplication()
{
    emit m_mbDataLoggerSignals->startListenForApplicationStateChanges();
    emit m_mbDataLoggerSignals->startModbusCommunication();

    m_isMbAppRunning = true;
    emit mbAppRunningChanged();
}

void InitialView::stopModbusApplication()
{
    emit m_mbDataLoggerSignals->stopModbusCommunication();
    emit m_mbDataLoggerSignals->stopListenForApplicationStateChanges();

    m_isMbAppRunning = false;
    emit mbAppRunningChanged();
}

void InitialView::onApplicationStateChanged(ApplicationState applicationState)
{
    switch (applicationState) {
        case ApplicationState::STARTING:
            setStartButtonVisibility(true);
            enableButton(false);
            break;

        case ApplicationState::STARTED:
            setStartButtonVisibility(false);
            enableButton(false);
            break;

        case ApplicationState::RUNNING:
            setStartButtonVisibility(false);
            enableButton(true);
            break;

        case ApplicationState::STOPPING:
            setStartButtonVisibility(false);
            enableButton(false);
            break;

        case ApplicationState::STOPPED:
            setStartButtonVisibility(true);
            enableButton(true);
            break;
    }
}

void InitialView::enableButton(bool enableIt)
{
    if (m_isButtonEnabled != enableIt) {
        m_isButtonEnabled = enableIt;
        emit buttonEnabledChanged();
    }
}

void InitialView::setStartButtonVisibility(bool isVisible)
{
    if (m_isStartButtonVisible != isVisible) {
        m_isStartButtonVisible = isVisible;
        emit startButtonVisibilityChanged();
    }
}

}
