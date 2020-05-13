#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include "gmock/gmock.h"

class MockModbusSlave : public Gateway::ModbusSlave
{
public:
    MOCK_METHOD(void, setModbusDataMapping, (const Gateway::ModbusDataMapping& mbMapping), (override));
    MOCK_METHOD(void, bind, (const std::string& ipAddr, const int port), (override));
    MOCK_METHOD(int, listen, (const int nbConns), (override));
    MOCK_METHOD(void, accept, (int& socket), (override));
    MOCK_METHOD(int, receive, (std::vector<uint8_t> & request), (override));
    MOCK_METHOD(int, reply, (std::vector<uint8_t> & request), (override));
    MOCK_METHOD(void, close, (), (override));
};
