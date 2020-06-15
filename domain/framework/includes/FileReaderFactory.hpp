#pragma once

#include "domain/framework/interfaces/ConfigurationFileReader.hpp"

#include <memory>

namespace Framework {

enum class FileReaderFramework
{
    NLOHMANN_JSON,
    OTHER_READER_FRAMEWORK
};

class FileReaderFactory
{
public:
    static std::shared_ptr<ConfigurationFileReader> createFileReader(FileReaderFramework fileReaderFramework);
};

}
