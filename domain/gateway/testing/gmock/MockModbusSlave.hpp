#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include "gmock/gmock.h"

class MockModbusSlave : public Gateway::ModbusSlave
{
public:
    MOCK_METHOD(bool, init, (const std::string&, const int, const Gateway::ModbusDataMapping&), (override));
    MOCK_METHOD(void, run, (), (override));
};
