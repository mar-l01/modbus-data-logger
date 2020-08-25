#pragma once

#include "domain/entity/includes/ModbusLoggerConfiguration.hpp"
#include "domain/logging/interfaces/FileLogger.hpp"

#include "gmock/gmock.h"

class MockFileLogger : public Logging::FileLogger
{
public:
    MOCK_METHOD(void, startLogging, (), (override));
    MOCK_METHOD(void, stopLogging, (), (override));
    MOCK_METHOD(void, changeLogFileConfiguration, (const Entity::ModbusLoggerConfiguration&), (override));
};
