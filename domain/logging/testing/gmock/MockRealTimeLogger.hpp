#pragma once

#include "domain/logging/interfaces/RealTimeLogger.hpp"

#include "gmock/gmock.h"

class MockRealTimeLogger : public Logging::RealTimeLogger
{
public:
    MOCK_METHOD(std::shared_ptr<Framework:: ::ScopedConnection>, addModbusRequestListener,
                (Framework:: ::SignalCallback<Entity::ModbusTcpRequest> signalCallback), (override));
    MOCK_METHOD(std::shared_ptr<Framework:: ::ScopedConnection>, addModbusResponseListener,
                (Framework:: ::SignalCallback<Entity::ModbusTcpResponse> signalCallback), (override));
};
