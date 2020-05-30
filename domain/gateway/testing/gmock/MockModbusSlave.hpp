#pragma once

#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include "gmock/gmock.h"

class MockModbusSlave : public Gateway::ModbusSlave
{
public:
    MOCK_METHOD(void, setModbusDataMapping, (const Entity::ModbusDataMapping&), (override));
    MOCK_METHOD(void, bind, (const std::string&, const int), (override));
    MOCK_METHOD(int, listen, (const int), (override));
    MOCK_METHOD(void, accept, (int&), (override));
    MOCK_METHOD(Gateway::ModbusReceiveStatus, receive, (std::shared_ptr<Entity::ModbusTcpRequest>&), (override));
    MOCK_METHOD(Gateway::ModbusReceiveStatus, reply, (std::shared_ptr<Entity::ModbusTcpResponse>&), (override));
    MOCK_METHOD(Gateway::ModbusReceiveStatus, replyException, (Entity::ModbusExceptionCode mbExceptionCode),
                (override));
    MOCK_METHOD(void, close, (), (override));
};
