#pragma once

#include "domain/logging/interfaces/DataLogger.hpp"

namespace Entity {
// forward declarations
class ModbusTcpRequest;
class ModbusTcpResponse;
}

namespace Logging {

class ModbusDataLogger : public DataLogger
{
public:
    void logModbusRequest(const Entity::ModbusTcpRequest& mbRequest) override;
    void logModbusResponse(const Entity::ModbusTcpResponse& mbResponse) override;
};

}
