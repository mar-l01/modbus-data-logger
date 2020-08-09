#pragma once

#include "domain/entity/includes/ModbusConfiguration.hpp"

#include <string>

namespace Framework {

/**
 * @brief This interfaces provides methods to read a Modbus configuration provided in
 * a pre-defined format (currently only json-files are supported) and return the read
 * file in an @ref Entity::ModbusConfiguration object.
 */
class ConfigurationFileReader
{
public:
    /**
     * @brief Read configuration file located at given path (filepath including filename).
     *
     * @param path Filepath to the configuration file, e.g. /usr/local/mbdl-config.json
     */
    virtual void readConfigurationFile(const std::string& path) = 0;

    /**
     * @brief Get the current @ref Entity::ModbusConfiguration object, which holds information about how to
     * configure this application.
     *
     * @return Entity::ModbusConfiguration A Modbus configuration used for this application.
     */
    virtual Entity::ModbusConfiguration getModbusConfiguration() const = 0;
};

}
