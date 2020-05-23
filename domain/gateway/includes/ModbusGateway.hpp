#pragma once

#include "domain/gateway/interfaces/ModbusRequestController.hpp"

namespace Gateway {

class ModbusGateway : public ModbusRequestController
{
public:
    ModbusGateway();
    std::shared_ptr<Entity::ModbusTcpResponse> forwardModbusRequestAndWaitForResponse(
      std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest) override;
};

}
