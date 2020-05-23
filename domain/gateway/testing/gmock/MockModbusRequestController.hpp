#pragma once

#include "domain/gateway/interfaces/ModbusRequestController.hpp"

#include "gmock/gmock.h"

class MockModbusRequestController : public Gateway::ModbusRequestController
{
public:
    MOCK_METHOD(std::shared_ptr<Entity::ModbusTcpResponse>, forwardModbusRequestAndWaitForResponse,
                (std::shared_ptr<Entity::ModbusTcpRequest>&), (override));
};
