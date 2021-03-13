#pragma once

#include "domain/gateway/interfaces/ModbusSlaveController.hpp"

#include "gmock/gmock.h"

class MockModbusSlaveController : public Gateway::ModbusSlaveController
{
public:
    MOCK_METHOD(void, waitForIncomingConnection, (), (override));
    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, disconnect, (), (override));
    MOCK_METHOD(bool, isExternalMasterConnected, (), (override));
};
