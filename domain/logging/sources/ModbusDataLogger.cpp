#include "domain/logging/includes/ModbusDataLogger.hpp"

#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

namespace Logging {

ModbusDataLogger::ModbusDataLogger() {}

void ModbusDataLogger::logModbusRequest(const Entity::ModbusTcpRequest& mbRequest) {}

void ModbusDataLogger::logModbusResponse(const Entity::ModbusTcpResponse& mbResponse) {}

ConnectionPointer ModbusDataLogger::addModbusRequestListener(
  const std::function<void(const Entity::ModbusTcpRequest& mbRequest)>& signalCallback)
{
    return ConnectionPointer();
}

ConnectionPointer ModbusDataLogger::addModbusResponseListener(
  const std::function<void(const Entity::ModbusTcpResponse& mbResponse)>& signalCallback)
{
    return ConnectionPointer();
}

}
