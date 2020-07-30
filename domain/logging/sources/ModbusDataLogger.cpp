#include "domain/logging/includes/ModbusDataLogger.hpp"

#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

namespace Logging {

ModbusDataLogger::ModbusDataLogger() {}

void ModbusDataLogger::logModbusRequest(const Entity::ModbusTcpRequest& mbRequest) {}

void ModbusDataLogger::logModbusResponse(const Entity::ModbusTcpResponse& mbResponse) {}

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

}
