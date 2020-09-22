#pragma once

#include "domain/application/includes/ModbusDataLoggerFrameworks.hpp"
#include "domain/logging/interfaces/FileLoggerController.hpp"

#include <memory>

namespace Framework {

/**
 * @brief Factory to create a file logger controller using e.g. spdlog framework.
 */
class FileLoggerControllerFactory
{
public:
    /**
     * @brief Create a file logger controller which is able to control the logging of messages to files.
     *
     * @param loggingFramework The respective logging framework which should be used
     * @return A shared-ptr instance of type @ref Logging::FileLoggerController.
     */
    static std::shared_ptr<Logging::FileLoggerController> createFileLoggerController(LoggingFramework loggingFramework);
};

}
