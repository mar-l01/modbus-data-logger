#include "domain/gateway/includes/ModbusGateway.hpp"

namespace Gateway {

ModbusGateway::ModbusGateway() {}

std::shared_ptr<Entity::ModbusTcpResponse> ModbusGateway::forwardModbusRequestAndWaitForResponse(
  std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest)
{
    // TODO(loipfingerm, 23.05.2020): use provided request to be able to extract correct numbers of read
    //      values from response
    return std::make_shared<Entity::ModbusTcpResponse>();
}

}
