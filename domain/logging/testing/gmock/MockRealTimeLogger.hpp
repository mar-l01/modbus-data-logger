#pragma once

#include "domain/logging/interfaces/RealTimeLogger.hpp"

#include "gmock/gmock.h"

class MockRealTimeLogger : public Logging::RealTimeLogger
{
public:
    MOCK_METHOD(std::shared_ptr<ScopedConnection>, addModbusRequestListener,
                (SignalCallback<Entity::ModbusTcpRequest> signalCallback), (override));
    MOCK_METHOD(std::shared_ptr<ScopedConnection>, addModbusResponseListener,
                (SignalCallback<Entity::ModbusTcpResponse> signalCallback), (override));
};
