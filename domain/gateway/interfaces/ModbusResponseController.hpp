#pragma once

#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include <functional>

namespace Gateway {

class ModbusResponseController
{
public:
    virtual std::shared_ptr<Entity::ModbusTcpResponse> getExternalModbusSlaveResponse(
      std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest) = 0;
};

}
