#include "domain/framework/includes/FileReaderFactory.hpp"

#include "domain/framework/includes/JsonFileReader.hpp"

namespace Framework {

std::shared_ptr<ConfigurationFileReader> FileReaderFactory::createFileReader(FileReaderFramework fileReaderFramework)
{
    std::shared_ptr<ConfigurationFileReader> fileReader;

    switch (fileReaderFramework) {
        case FileReaderFramework::NLOHMANN_JSON:
            fileReader = std::make_shared<Framework::JsonFileReader>();
            break;
        case FileReaderFramework::OTHER_READER_FRAMEWORK:
            std::cerr << "[FileReaderFactory] Failed to create file reader instance (framework not supported)\n";
            fileReader = nullptr;
            break;
    }

    return fileReader;
}

}
