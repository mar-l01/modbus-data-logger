#include "domain/gateway/includes/ModbusGateway.hpp"

namespace Gateway {

ModbusGateway::ModbusGateway() {}

ModbusTcpMessageFrame ModbusGateway::forwardModbusRequestAndWaitForResponse(const ModbusTcpMessageFrame& mbRequest)
{
    // TODO(loipfingerm, 23.05.2020): use provided request to be able to extract correct numbers of read
    //      values from response
    return ModbusTcpMessageFrame();
}

}
