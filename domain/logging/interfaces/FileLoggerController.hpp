#pragma once

#include <variant>

namespace Entity {
// forward declarations
class ModbusLoggerConfiguration;
class ModbusTcpRequest;
class ModbusTcpResponse;
}

namespace Logging {

class FileLoggerController
{
public:
    virtual void startLogger() = 0;
    virtual void stopLogger() = 0;
    virtual void setLogConfiguration(const Entity::ModbusLoggerConfiguration& mbLogConfig) = 0;

    virtual void logModbusData(
      const std::variant<Entity::ModbusTcpRequest, Entity::ModbusTcpResponse>& mbModbusData) = 0;
};

}
