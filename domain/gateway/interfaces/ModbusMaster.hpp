#pragma once

#include <string>
#include <utility>
#include <vector>

namespace Gateway {

enum class ModbusOperationStatus
{
    SUCCESS,
    FAIL
};

template<typename T>
using ModbusReadOperationResult = std::pair<ModbusOperationStatus, std::vector<T>>;

class ModbusMaster
{
public:
    virtual void connect(const std::string& ipAddr, const int port) = 0;

    // read operations (return read values together with the status of the operation)
    virtual ModbusReadOperationResult<uint8_t> readCoilValues(int startAddress, int nbValues) = 0;
    virtual ModbusReadOperationResult<uint8_t> readDiscreteInputValues(int startAddress, int nbValues) = 0;
    virtual ModbusReadOperationResult<uint16_t> readHoldingRegisterValues(int startAddress, int nbValues) = 0;
    virtual ModbusReadOperationResult<uint16_t> readInputRegisterValues(int startAddress, int nbValues) = 0;

    // write operations (return the status of the operation)
    virtual ModbusOperationStatus writeSingleCoilValue(int startAddress, uint8_t coilValue) = 0;
    virtual ModbusOperationStatus writeSingleHoldingRegisterValue(int startAddress, uint16_t registerValue) = 0;
    virtual ModbusOperationStatus writeMultipleCoilValues(int startAddress, const std::vector<uint8_t>& coilValues) = 0;
    virtual ModbusOperationStatus writeMultipleHoldingRegisterValues(int startAddress,
                                                                     const std::vector<uint16_t>& registerValues) = 0;

    virtual void close() = 0;
};

}
