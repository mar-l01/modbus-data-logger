#include "ui/facade/includes/ModbusDataLoggerThreadController.hpp"

#include "ui/facade/includes/ModbusDataLoggerFacadeUIWrapper.hpp"

namespace Facade {

ModbusDataLoggerThreadController::ModbusDataLoggerThreadController(
  std::unique_ptr<ModbusDataLoggerFacadeUIWrapper>&& mbDataLoggerFacadeUIWrapper)
    : m_mbDataLoggerFacadeUIWrapper(std::move(mbDataLoggerFacadeUIWrapper))
{
    m_mbDataLoggerFacadeUIWrapper->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, m_mbDataLoggerFacadeUIWrapper.get(), &QObject::deleteLater);

    // connect all signals to the respective slots representing the domain facade wrapper
    connect(this, &ModbusDataLoggerThreadController::startModbusCommunication, m_mbDataLoggerFacadeUIWrapper.get(),
            &ModbusDataLoggerFacadeUIWrapper::onStartModbusCommunication);
    connect(this, &ModbusDataLoggerThreadController::stopModbusCommunication, m_mbDataLoggerFacadeUIWrapper.get(),
            &ModbusDataLoggerFacadeUIWrapper::onStopModbusCommunication);
    connect(this, &ModbusDataLoggerThreadController::startLogger, m_mbDataLoggerFacadeUIWrapper.get(),
            &ModbusDataLoggerFacadeUIWrapper::onStartLogger);
    connect(this, &ModbusDataLoggerThreadController::stopLogger, m_mbDataLoggerFacadeUIWrapper.get(),
            &ModbusDataLoggerFacadeUIWrapper::onStopLogger);
    connect(this, &ModbusDataLoggerThreadController::getCurrentApplicationState, m_mbDataLoggerFacadeUIWrapper.get(),
            &ModbusDataLoggerFacadeUIWrapper::onGetCurrentApplicationState);
    connect(this, &ModbusDataLoggerThreadController::startListenForApplicationStateChanges,
            m_mbDataLoggerFacadeUIWrapper.get(),
            &ModbusDataLoggerFacadeUIWrapper::onStartListenForApplicationStateChanges);
    connect(this, &ModbusDataLoggerThreadController::stopListenForApplicationStateChanges,
            m_mbDataLoggerFacadeUIWrapper.get(),
            &ModbusDataLoggerFacadeUIWrapper::onStopListenForApplicationStateChanges);

    m_workerThread.start();
}

ModbusDataLoggerThreadController::~ModbusDataLoggerThreadController()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

}
