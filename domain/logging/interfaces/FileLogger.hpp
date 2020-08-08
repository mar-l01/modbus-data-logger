#pragma once

namespace Entity {
// forward declaration
class ModbusLoggerConfiguration;
}

namespace Logging {

/**
 * @brief This interface provides methods which are used when logging a Modbus communication to a file.
 */
class FileLogger
{
public:
    /**
     * @brief Indicates to start logging to a file.
     */
    virtual void startLogging() = 0;

    /**
     * @brief Indicates to stop logging to a file.
     */
    virtual void stopLogging() = 0;

    /**
     * @brief Allows for changing the configuration which is used for logging, e.g. filename/-path, ...
     *
     * @param mbLogConfig The configuration which sets-up the file logging (see @ref ModbusLoggerConfiguration).
     */
    virtual void changeLogFileConfiguration(const Entity::ModbusLoggerConfiguration& mbLogConfig) = 0;
};

}
