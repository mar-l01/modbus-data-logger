#include "domain/gateway/datatypes/includes/ModbusDataMapping.hpp"

namespace Gateway {

std::ostream& operator<<(std::ostream& os, const ModbusDataMapping& mbMapping)
{
    os << "ModbusMapping [start-address (decimal) | #values]: "
       << "[ " << mbMapping.startAddressCoils << " | " << mbMapping.nbCoils << " ]\n"
       << "[ " << mbMapping.startAddressDiscreteInputs << " | " << mbMapping.nbDiscreteInputs << " ]\n"
       << "[ " << mbMapping.startAddressHoldingRegisters << " | " << mbMapping.nbHoldingRegisters << " ]\n"
       << "[ " << mbMapping.startAddressInputRegisters << " | " << mbMapping.nbInputRegisters;
    return os;
}

}
