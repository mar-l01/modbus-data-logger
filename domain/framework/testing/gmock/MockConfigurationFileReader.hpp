#pragma once

#include "domain/framework/interfaces/ConfigurationFileReader.hpp"

#include "gmock/gmock.h"

class MockConfigurationFileReader : public Framework::ConfigurationFileReader
{
public:
    MOCK_METHOD(void, readConfigurationFile, (const std::string&), (override));

    MOCK_METHOD(Entity::ModbusConfiguration, getModbusConfiguration, (), (const, override));
};
