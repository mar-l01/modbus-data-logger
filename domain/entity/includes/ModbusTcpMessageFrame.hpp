#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

namespace Entity {

/**
 * @brief Class representing common parts of a Modbus frame (either request or response), i.e.
 * header information like transaction-id or protocol-id
 */
class ModbusTcpMessageFrame
{
public:
    ModbusTcpMessageFrame();

    /**
     * @brief Transaction id (16-bit): Used to synchronize Modbus messages (request and response)
     */
    uint16_t transactionIdentifier;

    /**
     * @brief Protocol id (16-bit): Modbus over TCP/IP is defined by 0x0000
     */
    uint16_t protocolIdentifier;

    /**
     * @brief Length field (16-bit): Length of all bytes to follow afterwards
     */
    uint16_t lengthField;

    /**
     * @brief Unit id (8-bit): This id is by default set to 0xff (not used in TCP/IP)
     */
    uint8_t unitIdentifier;

    /**
     * @brief Function code (8-bit): Defines the action to execute, e.g. read values or write values
     */
    uint8_t functionCode;

    /**
     * @brief Data (n-bit): Unique for each function code; array of 8-bits
     */
    std::vector<uint8_t> dataBytes;

    /**
     * @brief Get start address provided in Modbus frame's data field
     * This can be applied to read or write requests.
     *
     * @return start address of the respective data-type (16-bit)
     */
    uint16_t getStartAddress() const;

    /**
     * @brief Get number of values to read or write provided in Modbus frame's data field.
     * This can be applied to read or write requests.
     *
     * @return total number of values to read/write (16-bit)
     */
    uint16_t getNumberOfValuesToReadOrWrite() const;

    /**
     * @brief Get single value to write.
     * This can be applied to write requests.
     *
     * @return single value of the respective data-type to write (16-bit)
     */
    uint16_t getSingleValueToWrite() const;

    /**
     * @brief Checks whether a provided function code is reported by this application
     *
     * @return true Function code is supported
     * @return false Function code is currently not supported (might be in the future)
     */
    bool isFunctionCodeSupported() const;

    /**
     * @brief If a Modbus request with provided single-bit values is performed, these are extracted:
     * Bit values are transferred as 8-bits per byte and bits not used are stuffed with zeroes.
     *
     * @param startByte byte of the data-field where the bit-values start
     * @param nbBitValues number of bit-values to extract
     * @return std::vector<uint8_t> a byte-vector which holds a single bit value per vector-element
     */
    std::vector<uint8_t> extractBitValues(int startByte, int nbBitValues) const;

    /**
     * @brief If a Modbus request with provided register values is performed, these are extracted:
     * Register values are transferred as two bytes and hence need to be set together to 16-bit values.
     *
     * @param startByte byte of the data-field where the register values start
     * @param nbRegisterValues number of register values to extract
     * @return std::vector<uint16_t> a 16-bit vector which holds a single register per vector-element
     */
    std::vector<uint16_t> extractRegisterValues(int startByte, int nbRegisterValues) const;

    /**
     * @brief Transforms the members into a standard Modbus frame consisting of 8-bits only.
     *
     * @return std::vector<uint8_t> the Modbus frame, which was built by the gathered data
     */
    std::vector<uint8_t> asByteVector();

    /**
     * @brief Converts a provided Modbus frame (consists of 8-bits only) into this class, e.g. extracts
     * header information or data values.
     *
     * @param byteVector the Modbus frame which should be converted
     */
    void initFromByteVector(const std::vector<uint8_t>& byteVector);

    friend bool operator==(const ModbusTcpMessageFrame& mbMsgFrameA, const ModbusTcpMessageFrame& mbMsgFrameB);
    friend std::ostream& operator<<(std::ostream& os, const ModbusTcpMessageFrame& mbMsgFrame);
};

}
