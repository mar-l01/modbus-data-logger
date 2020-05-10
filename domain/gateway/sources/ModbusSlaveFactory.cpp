#include "domain/gateway/includes/ModbusSlaveFactory.hpp"

#include "domain/framework/includes/LibModbusSlave.hpp"

namespace Gateway {

std::unique_ptr<ModbusSlave> ModbusSlaveFactory::createModbusSlave(ModbusSlaveFramework mbSlaveFramework)
{
    std::unique_ptr<ModbusSlave> modbusSlaveInstance;

    switch (mbSlaveFramework) {
        case ModbusSlaveFramework::LIBMODBUS:
            modbusSlaveInstance = std::make_unique<Framework::LibModbusSlave>();
            break;
        case ModbusSlaveFramework::OTHER_MODBUS_FRAMEWORK:
            std::cerr << "[ModbusSlaveFactory] Failed to create Modbus slave instance (framework not supported)\n";
            modbusSlaveInstance = nullptr;
            break;
    }

    return std::move(modbusSlaveInstance);
}

}
