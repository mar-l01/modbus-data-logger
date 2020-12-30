#include "ui/views/includes/InitialView.hpp"

#include "domain/application/interfaces/ModbusDataLoggerFacade.hpp"

#include <QDebug>

namespace Views {

InitialView::InitialView(const std::shared_ptr<Application::ModbusDataLoggerFacade>& mbDataLoggerFacade,
                         QObject* parent)
    : QObject(parent)
    , m_mbDataLoggerFacade(mbDataLoggerFacade)
{}

void InitialView::startModbusApplication()
{
    qDebug() << "Starting Modbus application";
}

void InitialView::stopModbusApplication()
{
    qDebug() << "Stopping Modbus application";
}

}
