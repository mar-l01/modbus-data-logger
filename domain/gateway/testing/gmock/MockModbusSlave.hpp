#pragma once

#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include "gmock/gmock.h"

class MockModbusSlave : public Gateway::ModbusSlave
{
public:
    MOCK_METHOD(void, setModbusDataMapping, (const Gateway::ModbusDataMapping&), (override));
    MOCK_METHOD(void, bind, (const std::string&, const int), (override));
    MOCK_METHOD(int, listen, (const int), (override));
    MOCK_METHOD(void, accept, (int&), (override));
    MOCK_METHOD(int, receive, (Gateway::ModbusTcpMessageFrame&), (override));
    MOCK_METHOD(int, reply, (Gateway::ModbusTcpMessageFrame&), (override));
    MOCK_METHOD(void, close, (), (override));
};
