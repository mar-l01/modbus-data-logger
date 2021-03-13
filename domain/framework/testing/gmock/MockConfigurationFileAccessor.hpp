#pragma once

#include "domain/framework/interfaces/ConfigurationFileAccessor.hpp"

#include "gmock/gmock.h"

class MockConfigurationFileAccessor : public Framework::ConfigurationFileAccessor
{
public:
    MOCK_METHOD(void, readConfigurationFile, (const std::string&), (override));
    MOCK_METHOD(Entity::ModbusConfiguration, getModbusConfiguration, (), (const, override));
    MOCK_METHOD(void, writeConfigurationFile, (const Entity::ModbusConfiguration& mbConfig, const std::string& path),
                (override));
};
