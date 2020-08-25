#pragma once

#include "domain/application/interfaces/ModbusDataLoggerFacade.hpp"

#include "gmock/gmock.h"

class MockModbusDataLoggerFacade : public Application::ModbusDataLoggerFacade
{
public:
    MOCK_METHOD(void, startModbusCommunication, (), (override));
    MOCK_METHOD(void, stopModbusCommunication, (), (override));
    MOCK_METHOD(void, startLogger, (), (override));
    MOCK_METHOD(void, stopLogger, (), (override));
};