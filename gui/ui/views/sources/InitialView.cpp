#include "ui/views/includes/InitialView.hpp"

#include <QDebug>

namespace Views {

InitialView::InitialView(QObject* parent)
    : QObject(parent)
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
