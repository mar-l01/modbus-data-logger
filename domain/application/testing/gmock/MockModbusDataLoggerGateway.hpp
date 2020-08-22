#pragma once

#include "domain/application/interfaces/ModbusDataLoggerGateway.hpp"

#include "gmock/gmock.h"

class MockModbusDataLoggerGateway : public Application::ModbusDataLoggerGateway
{
public:
    MOCK_METHOD(void, startModbusCommunication, (), (override));
    MOCK_METHOD(void, stopModbusCommunication, (), (override));
    MOCK_METHOD(void, startLogger, (), (override));
    MOCK_METHOD(void, stopLogger, (), (override));
};
