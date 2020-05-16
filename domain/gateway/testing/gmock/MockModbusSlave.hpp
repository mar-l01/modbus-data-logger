#pragma once

#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include "gmock/gmock.h"

class MockModbusSlave : public Gateway::ModbusSlave
{
public:
    MOCK_METHOD(void, setModbusDataMapping, (const Gateway::ModbusDataMapping& mbMapping), (override));
    MOCK_METHOD(void, bind, (const std::string& ipAddr, const int port), (override));
    MOCK_METHOD(int, listen, (const int nbConns), (override));
    MOCK_METHOD(void, accept, (int& socket), (override));
    MOCK_METHOD(int, receive, (Gateway::ModbusMessageFrame & request), (override));
    MOCK_METHOD(int, reply, (Gateway::ModbusMessageFrame & request), (override));
    MOCK_METHOD(void, close, (), (override));
};
