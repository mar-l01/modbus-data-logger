#pragma once

#include "domain/gateway/interfaces/ModbusRequestController.hpp"

namespace Gateway {

class ModbusGateway : public ModbusRequestController
{
public:
    ModbusGateway();
    ModbusTcpMessageFrame forwardModbusRequestAndWaitForResponse(const ModbusTcpMessageFrame& mbRequest) override;
};

}
