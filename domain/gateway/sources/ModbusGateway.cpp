#include "domain/gateway/includes/ModbusGateway.hpp"

namespace Gateway {

ModbusGateway::ModbusGateway() {}

ModbusTcpMessageFrame ModbusGateway::forwardModbusRequestAndWaitForResponse(const ModbusTcpMessageFrame& mbRequest)
{
    return ModbusTcpMessageFrame();
}

}
