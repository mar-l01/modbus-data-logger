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

TEST(TestModbusDataLoggerFacadeFactory, createModbusDataLoggerFacadeFactory_loggerNullptr)
{
    // pre-condition: framework not implemented
    auto frameworkDependenciesError = frameworkDependencies;
    frameworkDependenciesError.loggingFramework = Framework::LoggingFramework::OTHER_LOGGING_FRAMEWORK;

    EXPECT_ANY_THROW(ModbusDataLoggerFacadeFactory::createModbusDataLoggerFacade(filepath, frameworkDependenciesError));
}

TEST(TestModbusDataLoggerFacadeFactory, createModbusDataLoggerFacadeFactory_fileReaderNullptr)
{
    // pre-condition: framework not implemented
    auto frameworkDependenciesError = frameworkDependencies;
    frameworkDependenciesError.fileReaderFramework = Framework::FileReaderFramework::OTHER_READER_FRAMEWORK;

    EXPECT_ANY_THROW(ModbusDataLoggerFacadeFactory::createModbusDataLoggerFacade(filepath, frameworkDependenciesError));
}

TEST(TestModbusDataLoggerFacadeFactory, createModbusDataLoggerFacadeFactory_mbMasterNullptr)
{
    // pre-condition: framework not implemented
    auto frameworkDependenciesError = frameworkDependencies;
    frameworkDependenciesError.modbusMasterFramework = Gateway::ModbusComponentsFramework::OTHER_MODBUS_FRAMEWORK;

    EXPECT_ANY_THROW(ModbusDataLoggerFacadeFactory::createModbusDataLoggerFacade(filepath, frameworkDependenciesError));
}

TEST(TestModbusDataLoggerFacadeFactory, createModbusDataLoggerFacadeFactory_mbSlaveNullptr)
{
    // pre-condition: framework not implemented
    auto frameworkDependenciesError = frameworkDependencies;
    frameworkDependenciesError.modbusSlaveFramework = Gateway::ModbusComponentsFramework::OTHER_MODBUS_FRAMEWORK;

    EXPECT_ANY_THROW(ModbusDataLoggerFacadeFactory::createModbusDataLoggerFacade(filepath, frameworkDependenciesError));
}

}
