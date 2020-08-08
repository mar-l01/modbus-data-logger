#pragma once

#include "domain/framework/interfaces/ConfigurationFileReader.hpp"

#include <nlohmann/json.hpp>

namespace Entity {
/**
 * @brief Used to convert a @ref ModbusConfiguration to a json-file.
 * See https://github.com/nlohmann/json#basic-usage for further information
 */
void to_json(nlohmann::json& j, const ModbusConfiguration& mbConfig);

/**
 * @brief Used to convert a json-file to a @ref ModbusConfiguration.
 * See https://github.com/nlohmann/json#basic-usage for further information
 */
void from_json(const nlohmann::json& j, ModbusConfiguration& mbConfig);
}

namespace Framework {

/**
 * @brief This class represents a json-file reader and is used to read a file containing
 * information about our Modbus application. This file is interpreted and a respective
 * @ref ModbusConfiguration object is created.
 */
class JsonFileReader : public ConfigurationFileReader
{
public:
    /**
     * @brief Construct a new JsonFileReader object.
     *
     */
    JsonFileReader();

    /**
     * @see ConfigurationFileReader::readConfigurationFile
     */
    void readConfigurationFile(const std::string& path) override;

    /**
     * @see ConfigurationFileReader::getModbusConfiguration
     */
    Entity::ModbusConfiguration getModbusConfiguration() const override;

private:
    Entity::ModbusConfiguration m_modbusConfiguration;
};

}
