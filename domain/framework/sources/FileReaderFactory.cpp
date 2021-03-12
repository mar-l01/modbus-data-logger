#include "domain/framework/includes/FileReaderFactory.hpp"

#include "domain/framework/includes/JsonFileAccessor.hpp"

#include "spdlog/spdlog.h"

namespace Framework {

std::shared_ptr<ConfigurationFileAccessor> FileReaderFactory::createFileReader(FileReaderFramework fileReaderFramework)
{
    std::shared_ptr<ConfigurationFileAccessor> fileReader;

    switch (fileReaderFramework) {
        case FileReaderFramework::NLOHMANN_JSON:
            fileReader = std::make_shared<Framework::JsonFileAccessor>();
            break;
        case FileReaderFramework::OTHER_READER_FRAMEWORK:
            SPDLOG_ERROR("Failed to create file reader instance (framework not supported)");
            fileReader = nullptr;
            break;
    }

    return fileReader;
}

}
