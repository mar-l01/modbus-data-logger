#pragma once

#include "domain/application/includes/ModbusDataLoggerFrameworks.hpp"
#include "domain/framework/interfaces/ConfigurationFileAccessor.hpp"

#include <memory>


namespace Framework {

/**
 * @brief Factory to create a file reader for a respective data-format, e.g. json.
 */
class FileReaderFactory
{
public:
    /**
     * @brief Create a file-reader which is capable of reading json-files.
     *
     * @param fileReaderFramework The respective data-type which the file-reader should process.
     * @return A shared-ptr instance of type @ref ConfigurationFileAccessor.
     */
    static std::shared_ptr<ConfigurationFileAccessor> createFileReader(FileReaderFramework fileReaderFramework);
};

}
