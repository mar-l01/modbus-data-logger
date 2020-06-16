#pragma once

#include "domain/entity/includes/ModbusConfiguration.hpp"

#include <string>

namespace Framework {

class ConfigurationFileReader
{
public:
    virtual void readConfigurationFile(const std::string& path) = 0;

    virtual Entity::ModbusConfiguration getModbusConfiguration() const = 0;
};

}
