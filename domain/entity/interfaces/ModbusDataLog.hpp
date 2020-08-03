#pragma once

#include <string>

namespace Entity {

class ModbusDataLog
{
public:
    virtual std::string convertToLogString() const = 0;
};

}
