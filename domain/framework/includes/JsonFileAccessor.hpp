#pragma once

#include "domain/framework/interfaces/ConfigurationFileAccessor.hpp"

#include <nlohmann/json.hpp>

namespace Entity {
/**
 * @brief Used to convert a @ref Entity::ModbusConfiguration to a json-file.
 * See https://github.com/nlohmann/json#basic-usage for further information
 */
void to_json(nlohmann::json& j, const ModbusConfiguration& mbConfig);

/**
 * @brief Used to convert a json-file to a @ref Entity::ModbusConfiguration.
 * See https://github.com/nlohmann/json#basic-usage for further information
 */
void from_json(const nlohmann::json& j, ModbusConfiguration& mbConfig);
}

namespace Framework {

/**
 * @brief This class represents a json-file reader and is used to read a file containing
 * information about our Modbus application. This file is interpreted and a respective
 * @ref Entity::ModbusConfiguration object is created.
 */
class JsonFileAccessor : public ConfigurationFileAccessor
{
public:
    /**
     * @brief Construct a new JsonFileAccessor object.
     *
     */
    JsonFileAccessor();

    /**
     * @see ConfigurationFileAccessor::readConfigurationFile
     */
    void readConfigurationFile(const std::string& path) override;

    /**
     * @see ConfigurationFileAccessor::getModbusConfiguration
     */
    Entity::ModbusConfiguration getModbusConfiguration() const override;

    /**
     * @see ConfigurationFileAccessor::writeConfigurationFile
     */
    void writeConfigurationFile(const Entity::ModbusConfiguration& mbConfig, const std::string& path) override;

private:
    Entity::ModbusConfiguration m_modbusConfiguration;
};

}
