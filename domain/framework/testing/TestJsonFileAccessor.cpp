#include "domain/framework/includes/JsonFileAccessor.hpp"

#include "gtest/gtest.h"

#include <filesystem>
#include <iostream>

namespace {

using namespace ::testing;
using namespace Framework;

class TestJsonFileAccessor : public ::testing::Test
{
protected:
    std::shared_ptr<JsonFileAccessor> createTestObject()
    {
        auto testObj = std::make_shared<JsonFileAccessor>();

        return testObj;
    }
};

TEST_F(TestJsonFileAccessor, ctorSuccessful)
{
    EXPECT_NO_THROW(JsonFileAccessor jFileReader);
}

TEST_F(TestJsonFileAccessor, readConfigurationFileNoThrowWhenUnkownFilepath)
{
    auto testObj = createTestObject();
    const auto filePath = "unkown";

    EXPECT_NO_THROW(testObj->readConfigurationFile(filePath));
}

TEST_F(TestJsonFileAccessor, getModbusConfigurationReturnsConfigObject)
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

TEST_F(TestJsonFileAccessor, getModbusConfigurationReturnsEmptyObject)
{
    auto testObj = createTestObject();
    const auto& expectedEmptyModbusConfig = testObj->getModbusConfiguration();

    EXPECT_EQ(Entity::ModbusConfiguration(), expectedEmptyModbusConfig);
}

TEST_F(TestJsonFileAccessor, writeModbusConfigurationAndReadIt)
{
    auto testObj = createTestObject();

    // make sure directory can be accessed (either from root or scripts directory)
    std::string fileDir = "../resources"; // from modbus-data-logger/scripts/
    if (std::filesystem::current_path().filename() == "modbus-data-logger") {
        fileDir = "resources"; // from modbus-data-logger/
    }
    std::string filePath = fileDir + "/mbdl_config_test.json";

    // create ModbusConfiguration which should be written
    Entity::ModbusConfiguration mbConfigToWrite;
    mbConfigToWrite.ipAddrIntSlave = "123.456.78.9";
    mbConfigToWrite.portIntSlave = 5432;
    mbConfigToWrite.ipAddrExtSlave = "127.0.0.1";
    mbConfigToWrite.portExtSlave = 21;

    mbConfigToWrite.dataMapping.startAddressCoils = 0;
    mbConfigToWrite.dataMapping.nbCoils = 10;
    mbConfigToWrite.dataMapping.startAddressDiscreteInputs = 100;
    mbConfigToWrite.dataMapping.nbDiscreteInputs = 10;
    mbConfigToWrite.dataMapping.startAddressHoldingRegisters = 1000;
    mbConfigToWrite.dataMapping.nbHoldingRegisters = 1000;
    mbConfigToWrite.dataMapping.startAddressInputRegisters = 255;
    mbConfigToWrite.dataMapping.nbInputRegisters = 5;

    mbConfigToWrite.modbusTimeout = 100;
    mbConfigToWrite.applicationTimeout = 100;

    ASSERT_NO_THROW(testObj->writeConfigurationFile(mbConfigToWrite, filePath));

    // read written file and check if configuration can be read-in correctly
    EXPECT_NO_THROW(testObj->readConfigurationFile(filePath));
    const auto& mbConfigRead = testObj->getModbusConfiguration();

    // check if configuration object was serialized/deserialized correctly
    EXPECT_EQ(mbConfigToWrite, mbConfigRead);

    // remove created test-file
    int result = remove(filePath.c_str());
}

}
