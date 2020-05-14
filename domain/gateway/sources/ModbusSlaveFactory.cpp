#include "domain/gateway/includes/ModbusSlaveFactory.hpp"

#include "domain/framework/includes/LibModbusSlave.hpp"

namespace Gateway {

std::shared_ptr<ModbusSlave> ModbusSlaveFactory::createModbusSlave(ModbusSlaveFramework mbSlaveFramework)
{
    std::shared_ptr<ModbusSlave> modbusSlaveInstance;

    switch (mbSlaveFramework) {
        case ModbusSlaveFramework::LIBMODBUS:
            modbusSlaveInstance = std::make_shared<Framework::LibModbusSlave>();
            break;
        case ModbusSlaveFramework::OTHER_MODBUS_FRAMEWORK:
            std::cerr << "[ModbusSlaveFactory] Failed to create Modbus slave instance (framework not supported)\n";
            modbusSlaveInstance = nullptr;
            break;
    }

    return modbusSlaveInstance;
}

}
