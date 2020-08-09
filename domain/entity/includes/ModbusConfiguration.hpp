#pragma once

#include "domain/entity/includes/ModbusDataMapping.hpp"

#include <iostream>
#include <string>

namespace Entity {
/**
 * @brief This class represents the Modbus configuration, which is
 * used to set-up the basic data logger application.
 */
class ModbusConfiguration
{
public:
    /**
     * @brief IP address of the internal Modbus slave (application side)
     */
    std::string ipAddrIntSlave;

    /**
     * @brief port number of the internal Modbus slave (application side)
     */
    int portIntSlave;

    /**
     * @brief IP address of the external Modbus slave (customer side)
     */
    std::string ipAddrExtSlave;

    /**
     * @brief port number of the external Modbus slave (customer side)
     */
    int portExtSlave;

    /**
     * @brief Modbus data mapping, which represents the start-address and
     * the total number of values of each respective data-types:
     * - coils
     * - discrete inputs
     * - holding register
     * - input register
     */
    Entity::ModbusDataMapping dataMapping;

    /**
     * @brief timeout value in ms specifying the time period in which the Modbus communication should stop
     */
    int modbusTimeout;

    /**
     * @brief timeout value in ms specifying the time period in which the application should stop
     */
    int applicationTimeout;

    friend bool operator==(const ModbusConfiguration& mbConfigA, const ModbusConfiguration& mbConfigB);
    friend std::ostream& operator<<(std::ostream& os, const ModbusConfiguration& mbMapping);
};

}
