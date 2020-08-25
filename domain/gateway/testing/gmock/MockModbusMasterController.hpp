#pragma once

#include "domain/gateway/interfaces/ModbusMasterController.hpp"

#include "gmock/gmock.h"

class MockModbusMasterController : public Gateway::ModbusMasterController
{
public:
    MOCK_METHOD(void, connect, (), (override));
    MOCK_METHOD(void, disconnect, (), (override));
};
