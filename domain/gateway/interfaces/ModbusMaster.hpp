#pragma once

#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include <string>
#include <vector>

namespace Gateway {

class ModbusMaster
{
public:
    virtual void connect(const std::string& ipAddr, const int port) = 0;
    virtual void reconnect() = 0;
    virtual void setResponseTimeout(const uint16_t timeoutInMs) = 0;

    // read operations (return read values together with the status of the operation)
    virtual Entity::ModbusReadOperationResult<uint8_t> readCoilValues(int startAddress, int nbValues) = 0;
    virtual Entity::ModbusReadOperationResult<uint8_t> readDiscreteInputValues(int startAddress, int nbValues) = 0;
    virtual Entity::ModbusReadOperationResult<uint16_t> readHoldingRegisterValues(int startAddress, int nbValues) = 0;
    virtual Entity::ModbusReadOperationResult<uint16_t> readInputRegisterValues(int startAddress, int nbValues) = 0;

    // write operations (return the status of the operation)
    virtual Entity::ModbusOperationStatus writeSingleCoilValue(int startAddress, uint8_t coilValue) = 0;
    virtual Entity::ModbusOperationStatus writeSingleHoldingRegisterValue(int startAddress, uint16_t registerValue) = 0;
    virtual Entity::ModbusOperationStatus writeMultipleCoilValues(int startAddress,
                                                                  const std::vector<uint8_t>& coilValues) = 0;
    virtual Entity::ModbusOperationStatus writeMultipleHoldingRegisterValues(
      int startAddress, const std::vector<uint16_t>& registerValues) = 0;

    virtual void close() = 0;
};

}
