#pragma once

#include "domain/gateway/interfaces/ModbusRequestController.hpp"

namespace Gateway {

class ModbusGateway : public ModbusRequestController
{
public:
    ModbusGateway();
    ModbusMessageFrame forwardModbusRequestAndWaitForResponse(const ModbusMessageFrame& mbRequest) override;
};

}
