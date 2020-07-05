#include "domain/framework/includes/FileReaderFactory.hpp"

#include "domain/framework/includes/JsonFileReader.hpp"

#include "spdlog/spdlog.h"

namespace Framework {

std::shared_ptr<ConfigurationFileReader> FileReaderFactory::createFileReader(FileReaderFramework fileReaderFramework)
{
    std::shared_ptr<ConfigurationFileReader> fileReader;

    switch (fileReaderFramework) {
        case FileReaderFramework::NLOHMANN_JSON:
            fileReader = std::make_shared<Framework::JsonFileReader>();
            break;
        case FileReaderFramework::OTHER_READER_FRAMEWORK:
            spdlog::error("[FileReaderFactory] Failed to create file reader instance (framework not supported)");
            fileReader = nullptr;
            break;
    }

    return fileReader;
}

}
