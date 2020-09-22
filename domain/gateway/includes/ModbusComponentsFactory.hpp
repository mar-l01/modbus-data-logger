#pragma once

#include "domain/application/includes/ModbusDataLoggerFrameworks.hpp"
#include "domain/gateway/interfaces/ModbusMaster.hpp"
#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include <memory>

namespace Gateway {

/**
 * @brief Factory to create a Modbus slave and a Modbus master using a defined framework, e.g. 'libmodbus'.
 */
class ModbusComponentsFactory
{
public:
    /**
     * @brief Create a @ref Gateway::ModbusSlave object with provided framework, which will act as a Modbus slave.
     *
     * @param mbFramework The respective framework which the Modbus slave should use.
     * @return A shared-ptr instance of type @ref Gateway::ModbusSlave.
     */
    static std::shared_ptr<ModbusSlave> createModbusSlave(ModbusComponentsFramework mbFramework);

    /**
     * @brief Create a @ref ModbusMaster object with provided framework, which will act as a Modbus master.
     *
     * @param mbFramework The respective framework which the Modbus master should use..
     * @return A shared-ptr instance of type @ref ModbusMaster.
     */
    static std::shared_ptr<ModbusMaster> createModbusMaster(ModbusComponentsFramework mbFramework);
};

}
