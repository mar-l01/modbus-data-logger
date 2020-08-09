#pragma once

#include <iostream>
#include <string>

namespace Entity {
/**
 * @brief Conversion factor of MB into KB: 1 MB is 1048576 KB
 */
constexpr const int ONE_MB_IN_KB = 1048576;

/**
 * @brief A configuration which specifies the logger:
 * a default configuration is provided.
 */
class ModbusLoggerConfiguration
{
public:
    /**
     * @brief Filepath of the resulting log file, including directory and filename
     */
    std::string logFilePath = "logs/modbus_log.txt";

    /**
     * @brief Name of the logger
     */
    std::string loggerName = "Modbus-Logger";

    /**
     * @brief Rotating log files require a maximum size of a log-file
     */
    int maxLogFileSizeInMb = 5 * ONE_MB_IN_KB;

    /**
     * @brief Rotating log files require a maximum number of rotating files
     */
    int maxNumberOfFiles = 3;

    friend bool operator==(const ModbusLoggerConfiguration& mbLogConfigA,
                           const ModbusLoggerConfiguration& mbLogConfigB);
    friend std::ostream& operator<<(std::ostream& os, const ModbusLoggerConfiguration& mbLogConfig);
};

}
