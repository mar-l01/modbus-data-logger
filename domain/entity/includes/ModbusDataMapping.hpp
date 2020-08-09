#pragma once

#include <iostream>

namespace Entity {
/**
 * @brief This class represents the Modbus data mapping, which specifies the
 * mapping which is used during the Modbus communication, i.e. start address
 * and number of the respective data types:
 * - coils
 * - discrete inputs
 * - holding register
 * - input register
 */
class ModbusDataMapping
{
public:
    /**
     * @brief Start address of the coil values
     */
    uint startAddressCoils;

    /**
     * @brief Start address of the discrete input values
     */
    uint startAddressDiscreteInputs;

    /**
     * @brief Start address of the holding register values
     */
    uint startAddressHoldingRegisters;

    /**
     * @brief Start address of the input register values
     */
    uint startAddressInputRegisters;

    /**
     * @brief Number of coil values
     */
    uint nbCoils;

    /**
     * @brief Number of discrete input values
     */
    uint nbDiscreteInputs;

    /**
     * @brief Number of holding register values
     */
    uint nbHoldingRegisters;

    /**
     * @brief Number of input register values
     */
    uint nbInputRegisters;

    friend bool operator==(const ModbusDataMapping& mbMappingA, const ModbusDataMapping& mbMappingB);
    friend std::ostream& operator<<(std::ostream& os, const ModbusDataMapping& mbMapping);
};

}
