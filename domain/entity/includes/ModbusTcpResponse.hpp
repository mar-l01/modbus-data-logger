#pragma once

#include "domain/entity/interfaces/ModbusDataLog.hpp"

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace Entity {

// forward declaration
class ModbusTcpRequest;

/**
 * @brief This enum represents the operation status of an incoming response.
 * It can either have succeeded, failed or a timeout occurred.
 */
enum class ModbusOperationStatus
{
    SUCCESS,
    FAIL,
    TIMEOUT
};

template<typename T>
using ModbusReadOperationResult = std::pair<ModbusOperationStatus, std::vector<T>>;

using ModbusReadValues = std::variant<std::vector<uint8_t>, std::vector<uint16_t>>;

/**
 * @brief This class represents a Modbus TCP response and provides additional methods
 * to extract information solely available in Modbus responses.
 * The inheritance of @ref ModbusDataLog is used to print a request in a log-file.
 */
class ModbusTcpResponse : public ModbusDataLog
{
public:
    /**
     * @brief Construct a new Modbus Tcp Request object (empty constructor).
     */
    ModbusTcpResponse();

    /**
     * @brief Construct a new Modbus Tcp Response object with provided arguments. These are denoting
     * the respective Modbus request @ref ModbusTcpRequest (which resulted in this Modbus response) and
     * the respective operation status @ref ModbusOperationStatus.
     *
     * @param mbRequest The respective Modbus TCP request, which created this response message
     * @param mbOpStatus The operation status: success, fail or timeout.
     */
    ModbusTcpResponse(const std::shared_ptr<ModbusTcpRequest>& mbRequest, const ModbusOperationStatus mbOpStatus);

    /**
     * @brief Get the current Modbus operation status
     *
     * @return a @ref ModbusOperationStatus (success, fail or timeout).
     */
    ModbusOperationStatus getModbusOperationStatus() const;

    /**
     * @brief Get all read bit values (either coil or discrete input values)
     *
     * @return a vector of all read bit values (8-bit elements each representing a single value)
     */
    std::vector<uint8_t> getReadBitValues() const;

    /**
     * @brief Get all read register values (either holding or input register values)
     *
     * @return a vector of all read register values (16-bit elements each representing a single value)
     */
    std::vector<uint16_t> getReadRegisterValues() const;

    /**
     * @brief Sets all read values, independent of their data-type:
     * - coil or discrete input values (one value per 8-bit)
     * - holding or input register values (16-bit)
     *
     * @param vals a std::variant which can either hold 8-bit or 16-bit values
     */
    void setReadValues(const ModbusReadValues& vals);

    friend bool operator==(const ModbusTcpResponse& mbResA, const ModbusTcpResponse& mbResB);

    /**
     * @see ModbusDataLog::convertToLogString
     */
    std::string convertToLogString() const override;

private:
    const std::shared_ptr<ModbusTcpRequest> m_mbRequest;
    ModbusOperationStatus m_operationStatus;
    ModbusReadValues m_readValues; // 1-bit or 16-bit values

    int computeLengthFieldInByte() const;
    std::vector<uint8_t> getDataBytesInByteVector() const;
};

}
