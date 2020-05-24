#include "domain/gateway/includes/ModbusComponentsFactory.hpp"

#include "domain/framework/includes/LibModbusMaster.hpp"
#include "domain/framework/includes/LibModbusSlave.hpp"

namespace Gateway {

std::shared_ptr<ModbusSlave> ModbusComponentsFactory::createModbusSlave(ModbusComponentsFramework mbFramework)
{
    std::shared_ptr<ModbusSlave> modbusSlaveInstance;

    switch (mbFramework) {
        case ModbusComponentsFramework::LIBMODBUS:
            modbusSlaveInstance = std::make_shared<Framework::LibModbusSlave>();
            break;
        case ModbusComponentsFramework::OTHER_MODBUS_FRAMEWORK:
            std::cerr << "[ModbusComponentsFactory] Failed to create Modbus slave instance (framework not supported)\n";
            modbusSlaveInstance = nullptr;
            break;
    }

    return modbusSlaveInstance;
}

std::shared_ptr<ModbusMaster> ModbusComponentsFactory::createModbusMaster(ModbusComponentsFramework mbFramework)
{
    std::shared_ptr<ModbusMaster> modbusMasterInstance;

    switch (mbFramework) {
        case ModbusComponentsFramework::LIBMODBUS:
            modbusMasterInstance = std::make_shared<Framework::LibModbusMaster>();
            break;
        case ModbusComponentsFramework::OTHER_MODBUS_FRAMEWORK:
            std::cerr
              << "[ModbusComponentsFactory] Failed to create Modbus master instance (framework not supported)\n";
            modbusMasterInstance = nullptr;
            break;
    }

    return modbusMasterInstance;
}

}
