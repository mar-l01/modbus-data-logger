#pragma once

#include <string>

namespace Framework {

class ConfigurationFileReader
{
    virtual void setConfigurationFilePath(const std::string& path) = 0;
};

}
