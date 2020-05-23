#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

namespace Entity {

namespace ModbusByteOffset {
constexpr const int START_BYTE_READ_VALUES = 1;
constexpr const int START_BYTE_WRITE_VALUES = 5;
}

class ModbusTcpMessageFrame
{
public:
    ModbusTcpMessageFrame();

    // Modbus request/response header values and dynamic data bytes array
    uint16_t transactionIdentifier;
    uint16_t protocolIdentifier;
    uint16_t lengthField;
    uint8_t unitIdentifier;
    uint8_t functionCode;
    std::vector<uint8_t> dataBytes;

    // common parts (request and response) of dynamic data bytes
    uint16_t getStartAddress() const;                // w, req/res and r, req
    uint16_t getNumberOfValuesToReadOrWrite() const; // r/w, req/res
    uint16_t getSingleValueToWrite() const;          // w, req/res

    std::vector<uint8_t> extractBitValues(int startByte, int nbBitValues) const;
    std::vector<uint16_t> extractRegisterValues(int startByte, int nbRegisterValues) const;
    std::vector<uint8_t> asByteVector();
    void initFromByteVector(const std::vector<uint8_t>& byteVector);

    friend bool operator==(const ModbusTcpMessageFrame& mbMsgFrameA, const ModbusTcpMessageFrame& mbMsgFrameB);
    friend std::ostream& operator<<(std::ostream& os, const ModbusTcpMessageFrame& mbMsgFrame);
};

}
