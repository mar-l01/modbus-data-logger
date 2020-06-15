#pragma once

#include "domain/entity/includes/ModbusDataMapping.hpp"

#include <string>

namespace Framework {

class ConfigurationFileReader
{
    virtual void setConfigurationFilePath(const std::string& path) = 0;

    // internal Modbus slave configuration
    virtual std::string getIpAddressInternalModbusSlave() const = 0;
    virtual int getPortInternalModbusSlave() const = 0;

    // external Modbus slave configuration
    virtual std::string getIpAddressExternalModbusSlave() const = 0;
    virtual int getPortExternalModbusSlave() const = 0;

    // Modbus data mapping
    virtual Entity::ModbusDataMapping getModbusDataMapping() const = 0;

    // common configuration
    virtual int getModbusTimeout() const = 0;
    virtual int getApplicationTimeout() const = 0;
};

}
