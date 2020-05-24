#pragma once

#include "domain/gateway/interfaces/ModbusMaster.hpp"
#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include <memory>

namespace Gateway {

enum class ModbusComponentsFramework
{
    LIBMODBUS,
    OTHER_MODBUS_FRAMEWORK
};

class ModbusComponentsFactory
{
public:
    static std::shared_ptr<ModbusSlave> createModbusSlave(ModbusComponentsFramework mbFramework);
    static std::shared_ptr<ModbusMaster> createModbusMaster(ModbusComponentsFramework mbFramework);
};

}
