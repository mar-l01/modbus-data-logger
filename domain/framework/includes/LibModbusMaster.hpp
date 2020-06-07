#pragma once

#include "domain/gateway/interfaces/ModbusMaster.hpp"

#include "modbus/modbus.h"

#include <functional>
#include <iostream>
#include <memory>

// use debug messages for libmodbus library
#define DEBUG

namespace Framework {

class LibModbusMaster : public Gateway::ModbusMaster
{
public:
    LibModbusMaster();

    void connect(const std::string& ipAddr, const int port);
    void setResponseTimeout(const uint16_t timeoutInMs);

    // read operations
    Entity::ModbusReadOperationResult<uint8_t> readCoilValues(int startAddress, int nbValues);
    Entity::ModbusReadOperationResult<uint8_t> readDiscreteInputValues(int startAddress, int nbValues);
    Entity::ModbusReadOperationResult<uint16_t> readHoldingRegisterValues(int startAddress, int nbValues);
    Entity::ModbusReadOperationResult<uint16_t> readInputRegisterValues(int startAddress, int nbValues);

    // write operations
    Entity::ModbusOperationStatus writeSingleCoilValue(int startAddress, uint8_t coilValue);
    Entity::ModbusOperationStatus writeSingleHoldingRegisterValue(int startAddress, uint16_t registerValue);
    Entity::ModbusOperationStatus writeMultipleCoilValues(int startAddress, const std::vector<uint8_t>& coilValues);
    Entity::ModbusOperationStatus writeMultipleHoldingRegisterValues(int startAddress,
                                                                     const std::vector<uint16_t>& registerValues);

    void close();

private:
    std::unique_ptr<modbus_t, std::function<void(modbus_t*)>> m_modbusContext;

    template<typename T>
    Entity::ModbusReadOperationResult<T> readValues(int (*libmodbusReadFunction)(modbus_t*, int, int, T*), int sAddr,
                                                    int nbVals);

    template<typename T>
    Entity::ModbusOperationStatus writeSingleValue(int (*libmodbusSingleWriteFunction)(modbus_t*, int, const T),
                                                   int sAddr, uint16_t value);

    template<typename T>
    Entity::ModbusOperationStatus writeValues(int (*libmodbusWriteFunction)(modbus_t*, int, int, const T*), int sAddr,
                                              std::vector<T> values);
};

}
