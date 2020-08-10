#include "domain/framework/includes/FileLoggerControllerFactory.hpp"

#include "domain/framework/includes/SpdlogFileController.hpp"

#include "spdlog/spdlog.h"

namespace Framework {

std::shared_ptr<Logging::FileLoggerController> FileLoggerControllerFactory::createFileLoggerController(
  LoggingFramework loggingFramework)
{
    std::shared_ptr<Logging::FileLoggerController> fileLoggerController;

    switch (loggingFramework) {
        case LoggingFramework::SPDLOG:
            fileLoggerController = std::make_shared<Framework::SpdlogFileController>();
            break;
        case LoggingFramework::OTHER_LOGGING_FRAMEWORK:
            SPDLOG_ERROR("Failed to create file logger controller instance (framework not supported)");
            fileLoggerController = nullptr;
            break;
    }

    return fileLoggerController;
}

}
