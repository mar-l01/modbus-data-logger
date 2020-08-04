#include "domain/logging/includes/ModbusDataLogger.hpp"

#include "domain/entity/includes/ModbusLoggerConfiguration.hpp"
#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include <thread>

namespace Logging {

ModbusDataLogger::ModbusDataLogger(const std::shared_ptr<FileLoggerController>& fileLoggerController)
    : m_fileLoggerController(fileLoggerController)
{}

void ModbusDataLogger::logModbusRequest(const Entity::ModbusTcpRequest& mbRequest)
{
    // run action in additional detached thread
    std::thread loggingThread([this, &mbRequest]() {
        // raise event
        m_mbRequestEvent(mbRequest);

        // log Modbus data
        m_fileLoggerController->logModbusData(std::make_shared<Entity::ModbusTcpRequest>(mbRequest));
    });
    loggingThread.detach();
}

void ModbusDataLogger::logModbusResponse(const Entity::ModbusTcpResponse& mbResponse)
{
    // run action in additional detached thread
    std::thread loggingThread([this, &mbResponse]() {
        // raise event
        m_mbResponseEvent(mbResponse);

        // log Modbus data
        m_fileLoggerController->logModbusData(std::make_shared<Entity::ModbusTcpResponse>(mbResponse));
    });
    loggingThread.detach();
}

std::shared_ptr<ScopedConnection> ModbusDataLogger::addModbusRequestListener(
  SignalCallback<Entity::ModbusTcpRequest> signalCallback)
{
    return std::make_shared<ScopedConnection>(m_mbRequestEvent.connect(signalCallback));
}

std::shared_ptr<ScopedConnection> ModbusDataLogger::addModbusResponseListener(
  SignalCallback<Entity::ModbusTcpResponse> signalCallback)
{
    return std::make_shared<ScopedConnection>(m_mbResponseEvent.connect(signalCallback));
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
