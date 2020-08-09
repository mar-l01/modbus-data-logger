#include "domain/framework/includes/FileLoggerControllerFactory.hpp"

#include "gtest/gtest.h"

namespace {

using namespace Framework;

TEST(TestFileLoggerControllerFactory, createFileLoggerControllerReturnSpdlogFileController)
{
    auto fileLoggerController = FileLoggerControllerFactory::createFileLoggerController(LoggingFramework::SPDLOG);
    EXPECT_NE(fileLoggerController, nullptr);
}

TEST(TestFileLoggerControllerFactory, createFileLoggerControllerReturnNullptr)
{
    auto noFileLoggerController =
      FileLoggerControllerFactory::createFileLoggerController(LoggingFramework::OTHER_LOGGING_FRAMEWORK);
    EXPECT_EQ(noFileLoggerController, nullptr);
}

}
