#include "domain/logging/includes/ModbusDataLogger.hpp"

#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

namespace Logging {

ModbusDataLogger::ModbusDataLogger() {}

void ModbusDataLogger::logModbusRequest(const Entity::ModbusTcpRequest& mbRequest) {}

void ModbusDataLogger::logModbusResponse(const Entity::ModbusTcpResponse& mbResponse) {}

}
