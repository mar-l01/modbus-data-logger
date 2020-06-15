#include "domain/entity/includes/ModbusConfiguration.hpp"

namespace Entity {

bool operator==(const ModbusConfiguration& mbConfigA, const ModbusConfiguration& mbConfigB)
{
    // clang-format off
    return mbConfigA.ipAddrIntSlave == mbConfigB.ipAddrIntSlave and
        mbConfigA.portIntSlave == mbConfigB.portIntSlave and
        mbConfigA.ipAddrExtSlave == mbConfigB.ipAddrExtSlave and
        mbConfigA.portExtSlave == mbConfigB.portExtSlave and
        mbConfigA.dataMapping == mbConfigB.dataMapping and
        mbConfigA.modbusTimeout == mbConfigB.modbusTimeout and
        mbConfigA.applicationTimeout == mbConfigB.applicationTimeout;
    // clang-format on
}

std::ostream& operator<<(std::ostream& os, const ModbusConfiguration& mbConfig)
{
    os << "ModbusConfiguration {\n"
       << "\tIP-address internal Modbus slave: " << mbConfig.ipAddrIntSlave << ",\n"
       << "\tPort internal Modbus slave: " << mbConfig.portIntSlave << ",\n"
       << "\tIP-address external Modbus slave: " << mbConfig.ipAddrExtSlave << ",\n"
       << "\tPort external Modbus slave: " << mbConfig.portExtSlave << ",\n"
       << "\tModbus data mapping: " << mbConfig.dataMapping << ",\n"
       << "\tModbus timeout: " << mbConfig.modbusTimeout << ",\n"
       << "\tApplication timeout: " << mbConfig.applicationTimeout << "\n"
       << "}\n";

    return os;
}

}
