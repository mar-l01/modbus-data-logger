#pragma once

#include <iostream>

namespace Entity {
class ModbusDataMapping
{
public:
    uint startAddressCoils;
    uint startAddressDiscreteInputs;
    uint startAddressHoldingRegisters;
    uint startAddressInputRegisters;

    uint nbCoils;
    uint nbDiscreteInputs;
    uint nbHoldingRegisters;
    uint nbInputRegisters;

    friend bool operator==(const ModbusDataMapping& mbMappingA, const ModbusDataMapping& mbMappingB);
    friend std::ostream& operator<<(std::ostream& os, const ModbusDataMapping& mbMapping);
};

}
