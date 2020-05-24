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
    LibModbusMaster();

    void connect(const std::string& ipAddr, const int port) = 0;

    // read operations
    Gateway::ModbusReadOperationResult<uint8_t> readCoilValues(int startAddress, int nbValues);
    Gateway::ModbusReadOperationResult<uint8_t> readDiscreteInputValues(int startAddress, int nbValues);
    Gateway::ModbusReadOperationResult<uint16_t> readHoldingRegisterValues(int startAddress, int nbValues);
    Gateway::ModbusReadOperationResult<uint16_t> readInputRegisterValues(int startAddress, int nbValues);

    // write operations
    Gateway::ModbusOperationStatus writeSingleCoilValue(int startAddress, uint8_t coilValue);
    Gateway::ModbusOperationStatus writeSingleHoldingRegisterValue(int startAddress, uint16_t registerValue);
    Gateway::ModbusOperationStatus writeMultipleCoilValues(int startAddress, const std::vector<uint8_t>& coilValues);
    Gateway::ModbusOperationStatus writeMultipleHoldingRegisterValues(int startAddress,
                                                                      const std::vector<uint16_t>& registerValues);

    void close();

private:
    std::unique_ptr<modbus_t, std::function<void(modbus_t*)>> m_modbusContext;

    template<typename T>
    Gateway::ModbusReadOperationResult<T> readValues(int (*libmodbusReadFunction)(modbus_t*, int, int, T*), int sAddr,
                                                     int nbVals);

    template<typename T>
    Gateway::ModbusOperationStatus writeSingleValue(int (*libmodbusSingleWriteFunction)(modbus_t*, int, const T),
                                                    int sAddr, uint16_t value);

    template<typename T>
    Gateway::ModbusOperationStatus writeValues(int (*libmodbusWriteFunction)(modbus_t*, int, int, const T*), int sAddr,
                                               std::vector<T> values);
};

}
