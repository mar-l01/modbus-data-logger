#pragma once

#include "domain/entity/includes/ModbusDataMapping.hpp"

#include <iostream>
#include <string>

namespace Entity {
class ModbusConfiguration
{
public:
    // internal Modbus slave settings
    std::string ipAddrIntSlave;
    int portIntSlave;

    // external Modbus slave settings
    std::string ipAddrExtSlave;
    int portExtSlave;

    // Modbus data mapping
    Entity::ModbusDataMapping dataMapping;

    // common application settings
    int modbusTimeout;
    int applicationTimeout;

    friend bool operator==(const ModbusConfiguration& mbConfigA, const ModbusConfiguration& mbConfigB);
    friend std::ostream& operator<<(std::ostream& os, const ModbusConfiguration& mbMapping);
};

}
