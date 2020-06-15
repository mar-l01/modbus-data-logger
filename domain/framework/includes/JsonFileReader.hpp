#pragma once

#include "domain/framework/interfaces/ConfigurationFileReader.hpp"

#include <nlohmann/json.hpp>

namespace Framework {

class JsonFileReader : public ConfigurationFileReader
{
public:
    JsonFileReader();

    void setConfigurationFilePath(const std::string& path) override;

    // internal Modbus slave configuration
    std::string getIpAddressInternalModbusSlave() const override;
    int getPortInternalModbusSlave() const override;

    // external Modbus slave configuration
    std::string getIpAddressExternalModbusSlave() const override;
    int getPortExternalModbusSlave() const override;

    // Modbus data mapping
    Entity::ModbusDataMapping getModbusDataMapping() const override;

    // common configuration
    int getModbusTimeout() const override;
    int getApplicationTimeout() const override;

private:
    std::string m_configurationFilePath;
};

}
