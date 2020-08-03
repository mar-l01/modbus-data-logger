#pragma once

#include "domain/logging/interfaces/FileLoggerController.hpp"

#include "gmock/gmock.h"

class MockFileLoggerController : public Logging::FileLoggerController
{
public:
    MOCK_METHOD(void, startLogger, (), (override));
    MOCK_METHOD(void, stopLogger, (), (override));
    MOCK_METHOD(void, setLogConfiguration, (const Entity::ModbusLoggerConfiguration&), (override));
    MOCK_METHOD(void, logModbusData, ((const std::variant<Entity::ModbusTcpRequest, Entity::ModbusTcpResponse>&)),
                (override));
};
