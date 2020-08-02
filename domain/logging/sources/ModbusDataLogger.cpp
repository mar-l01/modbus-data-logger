#include "domain/logging/includes/ModbusDataLogger.hpp"

#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include <thread>

namespace Logging {

ModbusDataLogger::ModbusDataLogger() {}

void ModbusDataLogger::logModbusRequest(const Entity::ModbusTcpRequest& mbRequest)
{
    // raise event in additonal thread
    std::thread loggingThread([this, &mbRequest]() { m_mbRequestEvent(mbRequest); });
    loggingThread.detach();
}

void ModbusDataLogger::logModbusResponse(const Entity::ModbusTcpResponse& mbResponse)
{
    // raise event in additonal thread
    std::thread loggingThread([this, &mbResponse]() { m_mbResponseEvent(mbResponse); });
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

void ModbusDataLogger::startLogging() {}

void ModbusDataLogger::stopLogging() {}

void ModbusDataLogger::changeLogFileConfiguration(const Entity::ModbusLoggerConfiguration& mbLogConfig) {}

}
