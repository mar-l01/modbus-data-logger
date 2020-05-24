#pragma once

#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include <functional>

namespace Gateway {

class ModbusRequestController
{
public:
    virtual std::shared_ptr<Entity::ModbusTcpResponse> forwardModbusRequestAndWaitForResponse(
      std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest) = 0;
};

}
