#pragma once

namespace Entity {
// forward declarations
class ModbusTcpRequest;
class ModbusTcpResponse;
}

namespace Logging {

class DataLogger
{
public:
    virtual void logModbusRequest(const Entity::ModbusTcpRequest& mbRequest) = 0;
    virtual void logModbusResponse(const Entity::ModbusTcpResponse& mbResponse) = 0;
};

}
