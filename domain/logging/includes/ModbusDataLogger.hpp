#pragma once

#include "domain/logging/interfaces/DataLogger.hpp"
#include "domain/logging/interfaces/RealTimeLogger.hpp"

namespace Entity {
// forward declarations
class ModbusTcpRequest;
class ModbusTcpResponse;
}

namespace Logging {

class ModbusDataLogger
    : public DataLogger
    , public RealTimeLogger
{
public:
    ModbusDataLogger();

    void logModbusRequest(const Entity::ModbusTcpRequest& mbRequest) override;
    void logModbusResponse(const Entity::ModbusTcpResponse& mbResponse) override;

    ConnectionPointer addModbusRequestListener(
      const std::function<void(const Entity::ModbusTcpRequest& mbRequest)>& signalCallback) override;
    ConnectionPointer addModbusResponseListener(
      const std::function<void(const Entity::ModbusTcpResponse& mbResponse)>& signalCallback) override;
};

}
