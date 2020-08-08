#pragma once

#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include <functional>

namespace Gateway {

/**
 * @brief This interface represents a controller to receive a response of an external Modbus slave. Together
 * with @ref ModbusRequestController these two interfaces are used to control the flow of the Modbus communication.
 */
class ModbusResponseController
{
public:
    /**
     * @brief Get a response message of an external Modbus slave. This method shall do the actual communication by
     * using the internal Modbus master instance to forward provided request and wait for the response and return it.
     *
     * @param mbRequest @ref ModbusTcpRequest for which a response should be returned.
     * @return A @ref ModbusTcpResponse which is triggered using above Modbus TCP request.
     */
    virtual std::shared_ptr<Entity::ModbusTcpResponse> getExternalModbusSlaveResponse(
      std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest) = 0;
};

}
