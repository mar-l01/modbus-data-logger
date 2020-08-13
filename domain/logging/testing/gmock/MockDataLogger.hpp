#pragma once

#include "domain/logging/interfaces/DataLogger.hpp"

#include "gmock/gmock.h"

class MockDataLogger : public Logging::DataLogger
{
public:
    MOCK_METHOD(void, logModbusRequest, (const std::shared_ptr<Entity::ModbusTcpRequest>&), (override));
    MOCK_METHOD(void, logModbusResponse, (const std::shared_ptr<Entity::ModbusTcpResponse>&), (override));
};
