#include "domain/application/includes/ModbusDataLoggerFacadeImpl.hpp"

#include "domain/gateway/interfaces/ModbusMasterController.hpp"
#include "domain/gateway/interfaces/ModbusSlaveController.hpp"
#include "domain/logging/interfaces/FileLogger.hpp"

namespace Application {

ModbusDataLoggerFacadeImpl::ModbusDataLoggerFacadeImpl(
  const std::shared_ptr<Gateway::ModbusMasterController>& mbMasterController,
  const std::shared_ptr<Gateway::ModbusSlaveController>& mbSlaveController,
  const std::shared_ptr<Logging::FileLogger>& fileLogger)
    : m_mbMasterController(mbMasterController)
    , m_mbSlaveController(mbSlaveController)
    , m_fileLogger(fileLogger)
{}

void ModbusDataLoggerFacadeImpl::startModbusCommunication()
{
    m_mbMasterController->connect();
    m_mbSlaveController->waitForIncomingConnection();

    // TODO(mar-l01, 28.08.2020): How to include run() method of ModbusSlaveController here?
}

void ModbusDataLoggerFacadeImpl::stopModbusCommunication()
{
    m_mbSlaveController->disconnect();
    m_mbMasterController->disconnect();
}

void ModbusDataLoggerFacadeImpl::startLogger()
{
    m_fileLogger->startLogging();
}

void ModbusDataLoggerFacadeImpl::stopLogger()
{
    m_fileLogger->stopLogging();
}

}
