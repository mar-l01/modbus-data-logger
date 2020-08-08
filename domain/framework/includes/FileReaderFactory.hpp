#pragma once

#include "domain/framework/interfaces/ConfigurationFileReader.hpp"

#include <memory>

namespace Framework {

/**
 * @brief A list of different frameworks to read different file formats.
 */
enum class FileReaderFramework
{
    NLOHMANN_JSON,
    OTHER_READER_FRAMEWORK
};

/**
 * @brief Factory to create a file reader for a respective data-format, e.g. json.
 */
class FileReaderFactory
{
public:
    /**
     * @brief Create a file-reader which is capable of reading json-files.
     *
     * @param fileReaderFramework The respective data-type which the file-reader should process (see @ref
     * FileReaderFramework).
     * @return A shared-ptr instance of type @ref ConfigurationFileReader.
     */
    static std::shared_ptr<ConfigurationFileReader> createFileReader(FileReaderFramework fileReaderFramework);
};

}
