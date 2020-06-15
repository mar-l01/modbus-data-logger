#pragma once

#include "domain/framework/interfaces/ConfigurationFileReader.hpp"

#include <nlohmann/json.hpp>

namespace Framework {

class JsonFileReader : public ConfigurationFileReader
{
public:
    JsonFileReader();

    void setConfigurationFilePath(const std::string& path);

private:
    std::string m_configurationFilePath;
};

}
