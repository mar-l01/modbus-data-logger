#include "domain/entity/includes/ModbusDataMapping.hpp"

namespace Entity {

bool operator==(const ModbusDataMapping& mbMappingA, const ModbusDataMapping& mbMappingB)
{
    // clang-format off
    return mbMappingA.nbCoils == mbMappingB.nbCoils and
        mbMappingA.nbDiscreteInputs == mbMappingB.nbDiscreteInputs and
        mbMappingA.nbHoldingRegisters == mbMappingB.nbHoldingRegisters and
        mbMappingA.nbInputRegisters == mbMappingB.nbInputRegisters;
    // clang-format on
}

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
