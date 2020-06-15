#include "domain/framework/includes/JsonFileReader.hpp"

namespace Framework {

JsonFileReader::JsonFileReader() {}

void JsonFileReader::setConfigurationFilePath(const std::string& path)
{
    return;
}
std::string JsonFileReader::getIpAddressInternalModbusSlave() const
{
    return "";
}

int JsonFileReader::getPortInternalModbusSlave() const
{
    return 0;
}

std::string JsonFileReader::getIpAddressExternalModbusSlave() const
{
    return "";
}

int JsonFileReader::getPortExternalModbusSlave() const
{
    return 0;
}

Entity::ModbusDataMapping JsonFileReader::getModbusDataMapping() const
{
    return Entity::ModbusDataMapping();
}

int JsonFileReader::getModbusTimeout() const
{
    return 0;
}

int JsonFileReader::getApplicationTimeout() const
{
    return 0;
}

}
