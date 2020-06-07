#pragma once

#include "domain/gateway/interfaces/ModbusMaster.hpp"

#include "gmock/gmock.h"

class MockModbusMaster : public Gateway::ModbusMaster
{
public:
    MOCK_METHOD(void, connect, (const std::string& ipAddr, const int port), (override));
    MOCK_METHOD(void, reconnect, (), (override));
    MOCK_METHOD(void, setResponseTimeout, (const uint16_t timeoutInMs), (override));
    MOCK_METHOD(Entity::ModbusReadOperationResult<uint8_t>, readCoilValues, (int startAddress, int nbValues),
                (override));
    MOCK_METHOD(Entity::ModbusReadOperationResult<uint8_t>, readDiscreteInputValues, (int startAddress, int nbValues),
                (override));
    MOCK_METHOD(Entity::ModbusReadOperationResult<uint16_t>, readHoldingRegisterValues,
                (int startAddress, int nbValues), (override));
    MOCK_METHOD(Entity::ModbusReadOperationResult<uint16_t>, readInputRegisterValues, (int startAddress, int nbValues),
                (override));
    MOCK_METHOD(Entity::ModbusOperationStatus, writeSingleCoilValue, (int startAddress, uint8_t coilValue), (override));
    MOCK_METHOD(Entity::ModbusOperationStatus, writeSingleHoldingRegisterValue,
                (int startAddress, uint16_t registerValue), (override));
    MOCK_METHOD(Entity::ModbusOperationStatus, writeMultipleCoilValues,
                (int startAddress, const std::vector<uint8_t>& coilValues), (override));
    MOCK_METHOD(Entity::ModbusOperationStatus, writeMultipleHoldingRegisterValues,
                (int startAddress, const std::vector<uint16_t>& registerValues), (override));

    MOCK_METHOD(void, close, (), (override));
};
