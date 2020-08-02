#pragma once

#include <iostream>
#include <string>

namespace Entity {
// 1 MB is 1048576 KB
constexpr const int ONE_MB_IN_KB = 1048576;

// default configuration for Modbus file logger
class ModbusLoggerConfiguration
{
public:
    std::string logFilePath = "logs/modbus_log.txt";
    std::string loggerName = "Modbus-Logger";

    // used for rotating log files
    int maxLogFileSizeInMb = 5 * ONE_MB_IN_KB;
    int maxNumberOfFiles = 3;

    friend bool operator==(const ModbusLoggerConfiguration& mbLogConfigA,
                           const ModbusLoggerConfiguration& mbLogConfigB);
    friend std::ostream& operator<<(std::ostream& os, const ModbusLoggerConfiguration& mbLogConfig);
};

}
