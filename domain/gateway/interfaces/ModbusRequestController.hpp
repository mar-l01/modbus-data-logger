#pragma once

#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include <functional>

namespace Gateway {

/**
 * @brief This interface represents a controller to forward Modbus requests to an external Modbus slave
 * and wait for a response of it.
 */
class ModbusRequestController
{
public:
    /**
     * @brief Forward provided Modbus request to a Modbus slave and wait for its response.
     *
     * @param mbRequest The Modbus TCP request (see @ref ModbusTcpRequest) which should be forwarded to the Modbus
     * slave.
     * @return The Modbus TCP response (see @ref ModbusTcpResponse) received from the Modbus slave.
     */
    virtual std::shared_ptr<Entity::ModbusTcpResponse> forwardModbusRequestAndWaitForResponse(
      std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest) = 0;
};

}
