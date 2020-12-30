#include "ui/views/includes/InitialView.hpp"

#include "domain/application/interfaces/ModbusDataLoggerFacade.hpp"

#include <QDebug>

namespace Views {

InitialView::InitialView(const std::shared_ptr<Application::ModbusDataLoggerFacade>& mbDataLoggerFacade,
                         QObject* parent)
    : QObject(parent)
    , m_mbDataLoggerFacade(mbDataLoggerFacade)
    , m_isMbAppRunning(false)
{}

void InitialView::startModbusApplication()
{
    qDebug() << "Starting Modbus application";
    m_mbDataLoggerFacade->startModbusCommunication();
    m_isMbAppRunning = true;
    emit mbAppRunningChanged();
}

void InitialView::stopModbusApplication()
{
    qDebug() << "Stopping Modbus application";
    m_mbDataLoggerFacade->stopModbusCommunication();
    m_isMbAppRunning = false;
    emit mbAppRunningChanged();
}

}
