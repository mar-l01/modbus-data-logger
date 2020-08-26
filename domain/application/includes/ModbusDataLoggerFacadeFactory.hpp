#pragma once

#include "domain/application/interfaces/ModbusDataLoggerFacade.hpp"
#include "domain/framework/includes/FileLoggerControllerFactory.hpp"
#include "domain/framework/includes/FileReaderFactory.hpp"
#include "domain/gateway/includes/ModbusComponentsFactory.hpp"

#include <memory>
#include <string>

namespace Application {

/**
 * @brief This datastructure holds all necessary dependencies to frameworks which implement the provided interfaces
 * using Dependency Injection.
 */
struct FrameworkDependencies
{
    Framework::LoggingFramework loggingFramework;
    Framework::FileReaderFramework fileReaderFramework;
    Gateway::ModbusComponentsFramework modbusMasterFramework;
    Gateway::ModbusComponentsFramework modbusSlaveFramework;
};

/**
 * @brief Factory to create a @ref ModbusDataLoggerFacade which holds all necessary dependencies to run the application.
 */
class ModbusDataLoggerFacadeFactory
{
    /**
     * @brief Create a ModbusDataLoggerFacade which has all components to run the Modbus data logger application.
     *
     * @param mbConfigFilepath Filepath to the Modbus configuration file
     * @param frameworkDependencies Struct holding all required framework dependencies
     * @return A shared-ptr instance of type @ref ModbusDataLoggerFacade.
     */
    static std::shared_ptr<ModbusDataLoggerFacade> createModbusDataLoggerFacade(
      const std::string& mbConfigFilepath, const FrameworkDependencies& frameworkDependencies);
};

}
