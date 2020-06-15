#pragma once

#include "domain/framework/interfaces/ConfigurationFileReader.hpp"

#include <nlohmann/json.hpp>

namespace Entity {
void to_json(nlohmann::json& j, const ModbusConfiguration& mbConfig);
void from_json(const nlohmann::json& j, ModbusConfiguration& mbConfig);
}

namespace Framework {

class JsonFileReader : public ConfigurationFileReader
{
public:
    JsonFileReader();

    void readConfigurationFile(const std::string& path) override;

    Entity::ModbusConfiguration getModbusConfiguration() const override;

private:
    Entity::ModbusConfiguration m_modbusConfiguration;
};

}
