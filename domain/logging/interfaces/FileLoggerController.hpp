#pragma once

#include <memory>

namespace Entity {
// forward declarations
class ModbusLoggerConfiguration;
class ModbusDataLog;
}

namespace Logging {

class FileLoggerController
{
public:
    virtual void startLogger() = 0;
    virtual void stopLogger() = 0;
    virtual void setLogConfiguration(const Entity::ModbusLoggerConfiguration& mbLogConfig) = 0;

    virtual void logModbusData(const std::shared_ptr<Entity::ModbusDataLog>& mbModbusData) = 0;
};

}
