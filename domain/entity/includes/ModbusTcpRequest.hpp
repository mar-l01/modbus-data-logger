#pragma once

#include "domain/entity/includes/ModbusTcpMessageFrame.hpp"
#include "domain/entity/interfaces/ModbusDataLog.hpp"

#include <memory>

namespace Entity {

/**
 * @brief This class represents a Modbus TCP request and provides additional methods
 * to extract information solely available in Modbus requests.
 * It inherits from @ref ModbusTcpMessageFrame to access all header-related information.
 * The inheritance of @ref ModbusDataLog is used to print a request in a log-file.
 */
class ModbusTcpRequest
    : public ModbusTcpMessageFrame
    , public ModbusDataLog
{
public:
    /**
     * @brief Construct a new Modbus Tcp Request object (empty constructor).
     */
    ModbusTcpRequest();

    /**
     * @brief Construct a new Modbus Tcp Request object. The provided 8-bit message frame
     * will be converted into a @ref ModbusTcpMessageFrame object.
     *
     * @param mbTcpReq A Modbus TCP request frame delivered as 8-bit array
     */
    ModbusTcpRequest(const std::vector<uint8_t>& mbTcpReq);

    /**
     * @brief Get number of bytes of values to write.
     *
     * @return total number of bytes of values which are read/written (as 8-bit)
     */
    uint8_t getNumberOfBytesOfValuesToWrite() const;

    /**
     * @brief Get all coil values which should be written
     *
     * @return a vector of 8-bit values, each representing a single coil value to write
     */
    std::vector<uint8_t> getCoilValuesToWrite() const;

    /**
     * @brief Get all holding register values which should be written
     *
     * @return a vector of 16-bit values, each representing a single holding register value to write
     */
    std::vector<uint16_t> getHoldingRegisterValuesToWrite() const;

    friend bool operator==(const ModbusTcpRequest& mbReqA, const ModbusTcpRequest& mbReqB);

    /**
     * @see ModbusDataLog::convertToLogString
     */
    std::string convertToLogString() const override;
};

}
