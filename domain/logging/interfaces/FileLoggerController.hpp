#pragma once

namespace Entity {
// forward declaration
class ModbusLoggerConfiguration;
}

namespace Logging {

class FileLoggerController
{
public:
    virtual void startLogger() = 0;
    virtual void stopLogger() = 0;
    virtual void setLogConfiguration(const Entity::ModbusLoggerConfiguration& mbLogConfig) = 0;
};

}
