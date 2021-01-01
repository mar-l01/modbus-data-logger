#include "ui/views/includes/InitialView.hpp"

#include "domain/application/interfaces/ModbusDataLoggerFacade.hpp"

namespace Views {

InitialView::InitialView(const std::shared_ptr<Application::ModbusDataLoggerFacade>& mbDataLoggerFacade,
                         QObject* parent)
    : QObject(parent)
    , m_mbDataLoggerFacade(mbDataLoggerFacade)
    , m_isMbAppRunning(false)
{}

void InitialView::startModbusApplication()
{
    m_mbDataLoggerFacade->startModbusCommunication();

    m_isMbAppRunning = true;
    emit mbAppRunningChanged();
}

void InitialView::stopModbusApplication()
{
    m_mbDataLoggerFacade->stopModbusCommunication();

    m_isMbAppRunning = false;
    emit mbAppRunningChanged();
}

}
