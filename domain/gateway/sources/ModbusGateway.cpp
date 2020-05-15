#include "domain/gateway/includes/ModbusGateway.hpp"

namespace Gateway {

ModbusGateway::ModbusGateway() {}

ModbusMessageFrame ModbusGateway::forwardModbusRequestAndWaitForResponse(const ModbusMessageFrame& mbRequest)
{
    return ModbusMessageFrame();
}

}
