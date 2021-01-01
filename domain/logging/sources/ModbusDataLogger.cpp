#include "domain/logging/includes/ModbusDataLogger.hpp"

#include "domain/entity/includes/ModbusLoggerConfiguration.hpp"
#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include <thread>

namespace Logging {

ModbusDataLogger::ModbusDataLogger(const std::shared_ptr<FileLoggerController>& fileLoggerController)
    : m_fileLoggerController(fileLoggerController)
{}

void ModbusDataLogger::logModbusRequest(const std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest)
{
    // run action in additional detached thread (copy mbRequest to thread lambda)
    std::thread loggingThread([this, mbRequest]() {
        // raise event
        m_mbRequestEvent(*mbRequest);

        // log Modbus data
        m_fileLoggerController->logModbusData(mbRequest);
    });
    loggingThread.detach();
}

void ModbusDataLogger::logModbusResponse(const std::shared_ptr<Entity::ModbusTcpResponse>& mbResponse)
{
    // run action in additional detached thread (copy mbResponse to thread lambda)
    std::thread loggingThread([this, mbResponse]() {
        // raise event
        m_mbResponseEvent(*mbResponse);

        // log Modbus data
        m_fileLoggerController->logModbusData(mbResponse);
    });
    loggingThread.detach();
}

std::shared_ptr<Framework::ScopedConnection> ModbusDataLogger::addModbusRequestListener(
  Framework::SignalCallback<Entity::ModbusTcpRequest> signalCallback)
{
    return std::make_shared<Framework::ScopedConnection>(m_mbRequestEvent.connect(signalCallback));
}

std::shared_ptr<Framework::ScopedConnection> ModbusDataLogger::addModbusResponseListener(
  Framework::SignalCallback<Entity::ModbusTcpResponse> signalCallback)
{
    return std::make_shared<Framework::ScopedConnection>(m_mbResponseEvent.connect(signalCallback));
}

void ModbusDataLogger::startLogging()
{
    m_fileLoggerController->startLogger();
}

void ModbusDataLogger::stopLogging()
{
    m_fileLoggerController->stopLogger();
}

void ModbusDataLogger::changeLogFileConfiguration(const Entity::ModbusLoggerConfiguration& mbLogConfig)
{
    m_fileLoggerController->setLogConfiguration(mbLogConfig);
}

}
