#pragma once

#include "domain/gateway/interfaces/ModbusRequestController.hpp"


namespace Gateway {

class ModbusGateway : public ModbusRequestController
{
public:
    ModbusGateway();
    std::shared_ptr<ModbusTcpResponse> forwardModbusRequestAndWaitForResponse(
      std::shared_ptr<ModbusTcpRequest>& mbRequest) override;
};

}
