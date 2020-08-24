#include "domain/application/includes/ModbusDataLoggerFacadeImpl.hpp"

#include "domain/gateway/includes/ModbusMasterController.hpp"
#include "domain/gateway/includes/ModbusSlaveController.hpp"
#include "domain/logging/includes/ModbusDataLogger.hpp"

namespace Application {

ModbusDataLoggerFacadeImpl::ModbusDataLoggerFacadeImpl(
  const std::shared_ptr<Gateway::ModbusMasterController>& mbMasterController,
  const std::shared_ptr<Gateway::ModbusSlaveController>& mbSlaveController,
  const std::shared_ptr<Logging::ModbusDataLogger>& dataLogger)
    : m_mbMasterController(mbMasterController)
    , m_mbSlaveController(mbSlaveController)
    , m_dataLogger(dataLogger)
{}

void ModbusDataLoggerFacadeImpl::startModbusCommunication()
{
    m_mbMasterController->connect();
}

void ModbusDataLoggerFacadeImpl::stopModbusCommunication()
{
    m_mbSlaveController->closeConnection();
    m_mbMasterController->closeConnection();
}

void ModbusDataLoggerFacadeImpl::startLogger()
{
    m_dataLogger->startLogging();
}

void ModbusDataLoggerFacadeImpl::stopLogger()
{
    m_dataLogger->stopLogging();
}

}
