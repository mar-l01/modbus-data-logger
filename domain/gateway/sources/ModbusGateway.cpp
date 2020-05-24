#include "domain/gateway/includes/ModbusGateway.hpp"

#include "domain/gateway/interfaces/ModbusResponseController.hpp"

namespace Gateway {

ModbusGateway::ModbusGateway(const std::shared_ptr<ModbusResponseController>& mbResponseController)
    : m_modbusResponseController(mbResponseController)
{}

std::shared_ptr<Entity::ModbusTcpResponse> ModbusGateway::forwardModbusRequestAndWaitForResponse(
  std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest)
{
    // TODO(loipfingerm, 23.05.2020): use provided request to be able to extract correct numbers of read
    //      values from response

    // forward Modbus request to controller which executes this request on external Modbus slave
    auto mbResponse = m_modbusResponseController->getExternalModbusSlaveResponse(mbRequest);

    return mbResponse;
}

}
