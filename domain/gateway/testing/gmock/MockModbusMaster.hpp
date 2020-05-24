#pragma once

#include "domain/gateway/interfaces/ModbusMaster.hpp"

#include "gmock/gmock.h"

class MockModbusMaster : public Gateway::ModbusMaster
{
public:
    MOCK_METHOD(void, connect, (const std::string& ipAddr, const int port), (override));
    MOCK_METHOD(Gateway::ModbusReadOperationResult<uint8_t>, readCoilValues, (int startAddress, int nbValues),
                (override));
    MOCK_METHOD(Gateway::ModbusReadOperationResult<uint8_t>, readDiscreteInputValues, (int startAddress, int nbValues),
                (override));
    MOCK_METHOD(Gateway::ModbusReadOperationResult<uint16_t>, readHoldingRegisterValues,
                (int startAddress, int nbValues), (override));
    MOCK_METHOD(Gateway::ModbusReadOperationResult<uint16_t>, readInputRegisterValues, (int startAddress, int nbValues),
                (override));
    MOCK_METHOD(Gateway::ModbusOperationStatus, writeSingleCoilValue, (int startAddress, uint8_t coilValue),
                (override));
    MOCK_METHOD(Gateway::ModbusOperationStatus, writeSingleHoldingRegisterValue,
                (int startAddress, uint16_t registerValue), (override));
    MOCK_METHOD(Gateway::ModbusOperationStatus, writeMultipleCoilValues,
                (int startAddress, const std::vector<uint8_t>& coilValues), (override));
    MOCK_METHOD(Gateway::ModbusOperationStatus, writeMultipleHoldingRegisterValues,
                (int startAddress, const std::vector<uint16_t>& registerValues), (override));

    MOCK_METHOD(void, close, (), (override));
};
