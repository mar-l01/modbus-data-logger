#pragma once

#include <memory>

namespace Entity {
// forward declarations
class ModbusTcpRequest;
class ModbusTcpResponse;
}

namespace Logging {

/**
 * @brief This interface provides methods to log either a Modbus request or a Modbus response.
 */
class DataLogger
{
public:
    /**
     * @brief Used to trigger a log of provided Modbus TCP request.
     *
     * @param mbRequest The Modbus TCP request to be logged.
     */
    virtual void logModbusRequest(const std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest) = 0;

    /**
     * @brief Used to trigger a log of provided Modbus TCP response.
     *
     * @param mbResponse The Modbus TCP response to be logged.
     */
    virtual void logModbusResponse(const std::shared_ptr<Entity::ModbusTcpResponse>& mbResponse) = 0;
};

}
