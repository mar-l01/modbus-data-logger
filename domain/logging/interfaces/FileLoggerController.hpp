#pragma once

#include <memory>

namespace Entity {
// forward declarations
class ModbusLoggerConfiguration;
class ModbusDataLog;
}

namespace Logging {

/**
 * @brief This interfaces enables a controller to be responsible for all file-logging.
 */
class FileLoggerController
{
public:
    /**
     * @brief Start file-logging.
     */
    virtual void startLogger() = 0;

    /**
     * @brief Stop file-logging.
     */
    virtual void stopLogger() = 0;

    /**
     * @brief Change the file logger configuration.
     *
     * @param mbLogConfig The configuration which sets-up the file logging (see @ref ModbusLoggerConfiguration).
     */
    virtual void setLogConfiguration(const Entity::ModbusLoggerConfiguration& mbLogConfig) = 0;

    /**
     * @brief Log provided Modbus data, either a Modbus request or a Modbus response.
     *
     * @param mbModbusData THe Modbus data to be logged. Needs to be dervied from @ref ModbusDataLog.
     */
    virtual void logModbusData(const std::shared_ptr<Entity::ModbusDataLog>& mbModbusData) = 0;
};

}
