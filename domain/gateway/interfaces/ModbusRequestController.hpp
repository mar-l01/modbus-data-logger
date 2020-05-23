#pragma once

#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include <functional>

namespace Gateway {

class ModbusRequestController
{
public:
    virtual ModbusTcpResponse forwardModbusRequestAndWaitForResponse(const ModbusTcpRequest& mbRequest) = 0;
};

}
