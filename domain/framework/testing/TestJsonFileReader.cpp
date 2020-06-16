#include "domain/framework/includes/JsonFileReader.hpp"

#include "gtest/gtest.h"

#include <filesystem>

namespace {

using namespace ::testing;
using namespace Framework;

class TestJsonFileReader : public ::testing::Test
{
protected:
    std::shared_ptr<JsonFileReader> createTestObject()
    {
        auto testObj = std::make_shared<JsonFileReader>();

        return testObj;
    }
};

TEST_F(TestJsonFileReader, ctorSuccessful)
{
    EXPECT_NO_THROW(JsonFileReader jFileReader);
}

TEST_F(TestJsonFileReader, readConfigurationFileNoThrowWhenUnkownFilepath)
{
    auto testObj = createTestObject();
    const auto filePath = "unkown";

    EXPECT_NO_THROW(testObj->readConfigurationFile(filePath));
}

TEST_F(TestJsonFileReader, getModbusConfigurationReturnsConfigObject)
{
    auto testObj = createTestObject();

    // make sure file can be accessed (either from root or scripts directory)
    auto filePath = "../resources/mbdl_config.json"; // from modbus-data-logger/scripts/
    if (std::filesystem::current_path().filename() == "modbus-data-logger") {
        filePath = "resources/mbdl_config.json"; // from modbus-data-logger/
    }

    ASSERT_NO_THROW(testObj->readConfigurationFile(filePath));
    const auto& returnedModbusConfig = testObj->getModbusConfiguration();

    Entity::ModbusConfiguration expectedModbusConfig;
    expectedModbusConfig.ipAddrIntSlave = "127.0.0.1";
    expectedModbusConfig.portIntSlave = 5002;
    expectedModbusConfig.ipAddrExtSlave = "127.0.0.1";
    expectedModbusConfig.portExtSlave = 5002;

    expectedModbusConfig.dataMapping.startAddressCoils = 0;
    expectedModbusConfig.dataMapping.nbCoils = 10;
    expectedModbusConfig.dataMapping.startAddressDiscreteInputs = 0;
    expectedModbusConfig.dataMapping.nbDiscreteInputs = 10;
    expectedModbusConfig.dataMapping.startAddressHoldingRegisters = 0;
    expectedModbusConfig.dataMapping.nbHoldingRegisters = 10;
    expectedModbusConfig.dataMapping.startAddressInputRegisters = 0;
    expectedModbusConfig.dataMapping.nbInputRegisters = 10;

    expectedModbusConfig.modbusTimeout = 200;
    expectedModbusConfig.applicationTimeout = 5000;

    EXPECT_EQ(expectedModbusConfig, returnedModbusConfig);
}

TEST_F(TestJsonFileReader, getModbusConfigurationReturnsEmptyObject)
{
    auto testObj = createTestObject();
    const auto& expectedEmptyModbusConfig = testObj->getModbusConfiguration();

    EXPECT_EQ(Entity::ModbusConfiguration(), expectedEmptyModbusConfig);
}

}
