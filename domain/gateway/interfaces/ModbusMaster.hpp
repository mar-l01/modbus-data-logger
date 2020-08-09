#pragma once

#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include <string>
#include <vector>

namespace Gateway {

/**
 * @brief This interface provides method required to set up a Modbus master.
 */
class ModbusMaster
{
public:
    /**
     * @brief Establish a connection to a Modbus slave, which listens on provided IP-address and port number.
     *
     * @param ipAddr IP-address to connect to, e.g. 192.168.12.12
     * @param port Port number to connect to, e.g. 502
     */
    virtual void connect(const std::string& ipAddr, const int port) = 0;

    /**
     * @brief Tries to reconnect, if a connectio gots lost.
     */
    virtual void reconnect() = 0;

    /**
     * @brief Set a Modbus response timeout, i.e. if no response is received within defined timeout the connection gets
     * closed.
     *
     * @param timeoutInMs Required timeout value in milliseconds.
     */
    virtual void setResponseTimeout(const uint16_t timeoutInMs) = 0;

    // read operations (return read values together with the status of the operation)
    /**
     * @brief Reads a certain number of coil values starting at provided start address.
     *
     * @param startAddress Address of first coil value to read.
     * @param nbValues Total number of coil values to read.
     * @return All read values as an array of 8-bit elements, each array-element represents a single read coil value.
     * Together with these read values an operation status is provided indicating if the read operation was successful.
     */
    virtual Entity::ModbusReadOperationResult<uint8_t> readCoilValues(int startAddress, int nbValues) = 0;

    /**
     * @brief Reads a certain number of discrete input values starting at provided start address.
     *
     * @param startAddress Address of first discrete input value to read.
     * @param nbValues Total number of discrete input values to read.
     * @return All read values as an array of 8-bit elements, each array-element represents a single read discrete input
     * value. Together with these read values an operation status is provided indicating if the read operation was
     * successful.
     */
    virtual Entity::ModbusReadOperationResult<uint8_t> readDiscreteInputValues(int startAddress, int nbValues) = 0;

    /**
     * @brief Reads a certain number of holding register values starting at provided start address.
     *
     * @param startAddress Address of first holding register value to read.
     * @param nbValues Total number of holding register values to read.
     * @return All read values as an array of 16-bit elements, each array-element represents a single read holding
     * register value. Together with these read values an operation status is provided indicating if the read operation
     * was successful.
     */
    virtual Entity::ModbusReadOperationResult<uint16_t> readHoldingRegisterValues(int startAddress, int nbValues) = 0;

    /**
     * @brief Reads a certain number of input register values starting at provided start address.
     *
     * @param startAddress Address of first input register value to read.
     * @param nbValues Total number of input register values to read.
     * @return All read values as an array of 16-bit elements, each array-element represents a single read input
     * register value. Together with these read values an operation status is provided indicating if the read operation
     * was successful.
     */
    virtual Entity::ModbusReadOperationResult<uint16_t> readInputRegisterValues(int startAddress, int nbValues) = 0;

    // write operations (return the status of the operation)
    /**
     * @brief  Writes a single coil value to given start address.
     *
     * @param startAddress Address where the coil value should be written to.
     * @param coilValue Bit-value which should be written (either 0 or 1).
     * @return An oepration status indicating if the write operation was successful.
     */
    virtual Entity::ModbusOperationStatus writeSingleCoilValue(int startAddress, uint8_t coilValue) = 0;

    /**
     * @brief  Writes a single holding register value to given start address.
     *
     * @param startAddress Address where the holding register value should be written to.
     * @param registerValue 16-bit-value which should be written.
     * @return An oepration status indicating if the write operation was successful.
     */
    virtual Entity::ModbusOperationStatus writeSingleHoldingRegisterValue(int startAddress, uint16_t registerValue) = 0;

    /**
     * @brief Writes all provided coil values to the start address and its following addresses.
     *
     * @param startAddress Address to where the first coil value should be written to.
     * @param coilValues All coil values which should be written, each array element shall represent one coil value
     * (either 0 or 1).
     * @return An operation status which indicates if the write operation was successful.
     */
    virtual Entity::ModbusOperationStatus writeMultipleCoilValues(int startAddress,
                                                                  const std::vector<uint8_t>& coilValues) = 0;

    /**
     * @brief Writes all provided holding register values to the start address and its following addresses.
     *
     * @param startAddress Address to where the first holding register value should be written to.
     * @param registerValues All holding register values which should be written, each array element shall represent one
     * holding register value.
     * @return An operation status which indicates if the write operation was successful.
     */
    virtual Entity::ModbusOperationStatus writeMultipleHoldingRegisterValues(
      int startAddress, const std::vector<uint16_t>& registerValues) = 0;

    /**
     * @brief Close the established connection to a Modbus slave.
     */
    virtual void close() = 0;
};

}
