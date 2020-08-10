#pragma once

#include "domain/logging/interfaces/FileLoggerController.hpp"

#include <memory>

namespace Framework {

/**
 * @brief A list of different frameworks which can be used for logging
 */
enum class LoggingFramework
{
    SPDLOG,
    OTHER_LOGGING_FRAMEWORK
};

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
