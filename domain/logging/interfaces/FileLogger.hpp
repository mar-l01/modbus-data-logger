#pragma once

namespace Entity {
// forward declaration
class ModbusLoggerConfiguration;
}

namespace Logging {

class FileLogger
{
public:
    virtual void startLogging() = 0;
    virtual void stopLogging() = 0;
    virtual void changeLogFileConfiguration(const Entity::ModbusLoggerConfiguration& mbLogConfig) = 0;
};

}
