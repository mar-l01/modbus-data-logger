#pragma once

#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include <functional>

namespace Gateway {

class ModbusRequestController
{
public:
    virtual ModbusMessageFrame forwardModbusRequestAndWaitForResponse(const ModbusMessageFrame& mbRequest) = 0;
};

}
