#pragma once

#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include <memory>

namespace Gateway {

enum class ModbusSlaveFramework
{
    LIBMODBUS,
    OTHER_MODBUS_FRAMEWORK
};

class ModbusSlaveFactory
{
public:
    static std::unique_ptr<ModbusSlave> createModbusSlave(ModbusSlaveFramework mbSlaveFramework);
};

}
