#include "ui/facade/includes/ModbusDataLoggerThreadController.hpp"

#include "ui/facade/includes/ModbusDataLoggerFacadeUIWrapper.hpp"
#include "ui/facade/includes/ModbusDataLoggerManager.hpp"

namespace Facade {

ModbusDataLoggerThreadController::ModbusDataLoggerThreadController(
  std::unique_ptr<ModbusDataLoggerFacadeUIWrapper>&& mbDataLoggerFacadeUIWrapper,
  const std::shared_ptr<ModbusDataLoggerManager> mbDataLoggerManager)
    : m_mbDataLoggerFacadeUIWrapper(std::move(mbDataLoggerFacadeUIWrapper))
    , m_mbDataLoggerManager(mbDataLoggerManager)
{
    m_mbDataLoggerFacadeUIWrapper->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, m_mbDataLoggerFacadeUIWrapper.get(), &QObject::deleteLater);

    // connect all signals to the respective slots representing the domain facade wrapper
    connect(m_mbDataLoggerManager.get(), &ModbusDataLoggerManager::startModbusCommunication,
            m_mbDataLoggerFacadeUIWrapper.get(), &ModbusDataLoggerFacadeUIWrapper::onStartModbusCommunication);
    connect(m_mbDataLoggerManager.get(), &ModbusDataLoggerManager::stopModbusCommunication,
            m_mbDataLoggerFacadeUIWrapper.get(), &ModbusDataLoggerFacadeUIWrapper::onStopModbusCommunication);
    connect(m_mbDataLoggerManager.get(), &ModbusDataLoggerManager::startLogger, m_mbDataLoggerFacadeUIWrapper.get(),
            &ModbusDataLoggerFacadeUIWrapper::onStartLogger);
    connect(m_mbDataLoggerManager.get(), &ModbusDataLoggerManager::stopLogger, m_mbDataLoggerFacadeUIWrapper.get(),
            &ModbusDataLoggerFacadeUIWrapper::onStopLogger);
    connect(m_mbDataLoggerManager.get(), &ModbusDataLoggerManager::getCurrentApplicationState,
            m_mbDataLoggerFacadeUIWrapper.get(), &ModbusDataLoggerFacadeUIWrapper::onGetCurrentApplicationState);
    connect(m_mbDataLoggerManager.get(), &ModbusDataLoggerManager::startListenForApplicationStateChanges,
            m_mbDataLoggerFacadeUIWrapper.get(),
            &ModbusDataLoggerFacadeUIWrapper::onStartListenForApplicationStateChanges);
    connect(m_mbDataLoggerManager.get(), &ModbusDataLoggerManager::stopListenForApplicationStateChanges,
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
