#include "domain/application/includes/ModbusDataLoggerFacadeFactory.hpp"

#include "gtest/gtest.h"

namespace {

using namespace Application;

const std::string filepath = "test/file.json"; // dummy file-path with no information
const FrameworkDependencies frameworkDependencies(Framework::LoggingFramework::SPDLOG,
                                                  Framework::FileReaderFramework::NLOHMANN_JSON,
                                                  Gateway::ModbusComponentsFramework::LIBMODBUS,
                                                  Gateway::ModbusComponentsFramework::LIBMODBUS);

TEST(TestModbusDataLoggerFacadeFactory, createModbusDataLoggerFacadeFactoryReturnModbusDataLoggerFacade)
{
    auto mbDataLoggerFacade =
      ModbusDataLoggerFacadeFactory::createModbusDataLoggerFacade(filepath, frameworkDependencies);
    EXPECT_NE(mbDataLoggerFacade, nullptr);
}

}
