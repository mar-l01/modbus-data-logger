#pragma once

#include "domain/gateway/interfaces/ModbusResponseController.hpp"

#include "gmock/gmock.h"

class MockModbusResponseController : public Gateway::ModbusResponseController
{
public:
    MOCK_METHOD(std::shared_ptr<Entity::ModbusTcpResponse>, getExternalModbusSlaveResponse,
                (std::shared_ptr<Entity::ModbusTcpRequest>&), (override));
};
