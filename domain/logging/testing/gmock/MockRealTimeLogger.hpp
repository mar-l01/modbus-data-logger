#pragma once

#include "domain/logging/interfaces/RealTimeLogger.hpp"

#include "gmock/gmock.h"

class MockRealTimeLogger : public Logging::RealTimeLogger
{
public:
    MOCK_METHOD(std::shared_ptr<Logging::ScopedConnection>, addModbusRequestListener,
                (Logging::SignalCallback<Entity::ModbusTcpRequest> signalCallback), (override));
    MOCK_METHOD(std::shared_ptr<Logging::ScopedConnection>, addModbusResponseListener,
                (Logging::SignalCallback<Entity::ModbusTcpResponse> signalCallback), (override));
};
