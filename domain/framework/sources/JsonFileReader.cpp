#include "domain/framework/includes/JsonFileReader.hpp"

namespace Framework {

JsonFileReader::JsonFileReader() {}

void JsonFileReader::readConfigurationFile(const std::string& path)
{
    return;
}

Entity::ModbusConfiguration JsonFileReader::getModbusConfiguration() const
{
    return Entity::ModbusConfiguration();
}

}
