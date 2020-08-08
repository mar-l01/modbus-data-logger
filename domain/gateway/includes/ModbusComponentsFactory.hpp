#pragma once

#include "domain/gateway/interfaces/ModbusMaster.hpp"
#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include <memory>

namespace Gateway {

/**
 * @brief A list of different frameworks to be used to create a Modbus slave or master
 */
enum class ModbusComponentsFramework
{
    LIBMODBUS,
    OTHER_MODBUS_FRAMEWORK
};

/**
 * @brief Factory to create a Modbus slave and a Modbus master using a defined framework, e.g. 'libmodbus'.
 */
class ModbusComponentsFactory
{
public:
    /**
     * @brief Create a @ref ModbusSlave object with provided framework, which will act as a Modbus slave.
     *
     * @param mbFramework The respective framework which the Modbus slave should use (see @ref
     * ModbusComponentsFramework).
     * @return A shared-ptr instance of type @ref ModbusSlave.
     */
    static std::shared_ptr<ModbusSlave> createModbusSlave(ModbusComponentsFramework mbFramework);

    /**
     * @brief Create a @ref ModbusMaster object with provided framework, which will act as a Modbus master.
     *
     * @param mbFramework The respective framework which the Modbus master should use (see @ref
     * ModbusComponentsFramework).
     * @return A shared-ptr instance of type @ref ModbusMaster.
     */
    static std::shared_ptr<ModbusMaster> createModbusMaster(ModbusComponentsFramework mbFramework);
};

}
