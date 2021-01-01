#pragma once

#ifdef _SHARED_LIBRARY_
#include "ModbusDataLoggerFacade.hpp"
#include "ModbusDataLoggerFrameworks.hpp"
#else
#include "domain/application/includes/ModbusDataLoggerFrameworks.hpp"
#include "domain/application/interfaces/ModbusDataLoggerFacade.hpp"
#endif

#include <memory>
#include <string>

namespace Framework {
class FileLoggerControllerFactory;
class FileReaderFactory;
}

namespace Gateway {
class ModbusComponentsFactory;
}

namespace Application {

/**
 * @brief This datastructure holds all necessary dependencies to frameworks which implement the provided interfaces
 * using Dependency Injection.
 */
struct FrameworkDependencies
{
    FrameworkDependencies(Framework::LoggingFramework logFramework, Framework::FileReaderFramework fReaderFramework,
                          Gateway::ModbusComponentsFramework mbMasterFramework,
                          Gateway::ModbusComponentsFramework mbSlaveFramework)
        : loggingFramework(logFramework)
        , fileReaderFramework(fReaderFramework)
        , modbusMasterFramework(mbMasterFramework)
        , modbusSlaveFramework(mbSlaveFramework)
    {}

    Framework::LoggingFramework loggingFramework;
    Framework::FileReaderFramework fileReaderFramework;
    Gateway::ModbusComponentsFramework modbusMasterFramework;
    Gateway::ModbusComponentsFramework modbusSlaveFramework;
};

/**
 * @brief Enum that holds different application states, which will be set in following order:
 * - start application -----------------> STARTING
 * - application is up -----------------> STARTED
 * - external Modbus device connected --> RUNNING
 * - stop application ------------------> STOPPING
 * - application is stopped ------------> STOPPED
 */
enum class APPLICATION_STATE : unsigned char
{
    STARTING = 0,
    STARTED,
    RUNNING,
    STOPPING,
    STOPPED
};

/**
 * @brief Factory to create a @ref ModbusDataLoggerFacade which holds all necessary dependencies to run the application.
 */
class ModbusDataLoggerFacadeFactory
{
public:
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
