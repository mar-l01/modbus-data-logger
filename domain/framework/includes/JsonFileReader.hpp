#pragma once

#include "domain/framework/interfaces/ConfigurationFileReader.hpp"

namespace Framework {

class JsonFileReader : public ConfigurationFileReader
{
public:
    JsonFileReader();

    void readConfigurationFile(const std::string& path) override;

    Entity::ModbusConfiguration getModbusConfiguration() const override;
};

}
