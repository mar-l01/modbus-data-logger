#pragma once

#include <string>

namespace Entity {

/**
 * @brief This interface provides a single method which is used for logging Modbus messages
 */
class ModbusDataLog
{
public:
    /**
     * @brief This methods needs to be implemented and will be used to log a Modbus message (request or response)
     *
     * @return a std::string representation of the Modbus message
     */
    virtual std::string convertToLogString() const = 0;
};

}
