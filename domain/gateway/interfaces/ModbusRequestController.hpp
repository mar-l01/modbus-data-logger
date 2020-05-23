#pragma once

#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include <functional>

namespace Gateway {

class ModbusRequestController
{
public:
    virtual std::shared_ptr<Entity::ModbusTcpResponse> forwardModbusRequestAndWaitForResponse(
      std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest) = 0;
};

}
