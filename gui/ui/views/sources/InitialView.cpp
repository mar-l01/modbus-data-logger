#include "ui/views/includes/InitialView.hpp"

#include "ui/facade/includes/ModbusDataLoggerSignals.hpp"

#include <QDebug>

namespace Views {

InitialView::InitialView(const std::shared_ptr<Facade::ModbusDataLoggerSignals>& mbDataLoggerSignals)
    : m_mbDataLoggerSignals(mbDataLoggerSignals)
    , m_isMbAppRunning(false)
{
    connect(m_mbDataLoggerSignals.get(), &Facade::ModbusDataLoggerSignals::applicationStateChanged, this,
            &InitialView::onApplicationStateChanged);
}

void InitialView::startModbusApplication()
{
    emit m_mbDataLoggerSignals->startModbusCommunication();
    emit m_mbDataLoggerSignals->startListenForApplicationStateChanges();

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

void InitialView::onApplicationStateChanged(Application::ApplicationState applicationState)
{
    qDebug() << "New application state: " << static_cast<int>(applicationState);
}

}
