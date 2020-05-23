#pragma once

#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include <functional>

namespace Gateway {

class ModbusRequestController
{
public:
    virtual std::shared_ptr<ModbusTcpResponse> forwardModbusRequestAndWaitForResponse(
      std::shared_ptr<ModbusTcpRequest>& mbRequest) = 0;
};

}
