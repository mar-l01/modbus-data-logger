#include "domain/entity/includes/ModbusLoggerConfiguration.hpp"

namespace Entity {

bool operator==(const ModbusLoggerConfiguration& mbLogConfigA, const ModbusLoggerConfiguration& mbLogConfigB)
{
    // clang-format off
    return mbLogConfigA.logFilePath == mbLogConfigB.logFilePath and
        mbLogConfigA.loggerName == mbLogConfigB.loggerName and
        mbLogConfigA.maxLogFileSizeInMb == mbLogConfigB.maxLogFileSizeInMb and
        mbLogConfigA.maxNumberOfFiles == mbLogConfigB.maxNumberOfFiles;
    // clang-format on
}

std::ostream& operator<<(std::ostream& os, const ModbusLoggerConfiguration& mbLogConfig)
{
    os << "ModbusConfiguration {\n"
       << "\tPath of logging file: " << mbLogConfig.logFilePath << ",\n"
       << "\tName of logger: " << mbLogConfig.loggerName << ",\n"
       << "\tMax. size of logging file in MB: " << mbLogConfig.maxLogFileSizeInMb << ",\n"
       << "\tMax. number of logging files: " << mbLogConfig.maxNumberOfFiles << '\n'
       << "}\n";

    return os;
}

}
