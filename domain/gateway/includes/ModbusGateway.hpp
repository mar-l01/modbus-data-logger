#pragma once

#include "domain/gateway/interfaces/ModbusRequestController.hpp"

namespace Gateway {

class ModbusResponseController;

class ModbusGateway : public ModbusRequestController
{
public:
    ModbusGateway(const std::shared_ptr<ModbusResponseController>& mbResponseController);

    std::shared_ptr<Entity::ModbusTcpResponse> forwardModbusRequestAndWaitForResponse(
      std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest) override;

private:
    std::shared_ptr<ModbusResponseController> m_modbusResponseController;
};

}
