#pragma once

#include "domain/gateway/interfaces/ModbusRequestController.hpp"

namespace Gateway {

class ModbusGateway : public ModbusRequestController
{
public:
    ModbusGateway();
    ModbusTcpResponse forwardModbusRequestAndWaitForResponse(const ModbusTcpRequest& mbRequest) override;
};

}
