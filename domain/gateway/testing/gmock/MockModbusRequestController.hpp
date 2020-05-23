#pragma once

#include "domain/gateway/interfaces/ModbusRequestController.hpp"

#include "gmock/gmock.h"

class MockModbusRequestController : public Gateway::ModbusRequestController
{
public:
    MOCK_METHOD(Gateway::ModbusTcpResponse, forwardModbusRequestAndWaitForResponse, (const Gateway::ModbusTcpRequest&),
                (override));
};
