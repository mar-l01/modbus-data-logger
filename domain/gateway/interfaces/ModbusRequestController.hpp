#pragma once

#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include <functional>

namespace Gateway {

class ModbusRequestController
{
public:
    virtual ModbusTcpMessageFrame forwardModbusRequestAndWaitForResponse(const ModbusTcpMessageFrame& mbRequest) = 0;
};

}
