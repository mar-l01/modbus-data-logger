#pragma once

#include <iostream>

namespace Gateway {
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

    friend std::ostream& operator<<(std::ostream& os, const ModbusDataMapping& mbMapping);
};

}
