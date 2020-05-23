#include <cstdint>
#include <iostream>
#include <vector>

namespace Gateway {

namespace ModbusByteOffset {
constexpr const int START_BYTE_READ_VALUES = 1;
constexpr const int START_BYTE_WRITE_VALUES = 5;
}

class ModbusTcpMessageFrame
{
public:
    ModbusTcpMessageFrame();

    uint16_t transactionIdentifier;
    uint16_t protocolIdentifier;
    uint16_t lengthField;
    uint8_t unitIdentifier;
    uint8_t functionCode;
    std::vector<uint8_t> dataBytes;

    uint16_t getStartAddress() const;                              // w, req/res and r, req
    uint16_t getNumberOfValuesToReadOrWrite() const;               // r/w, req/res
    uint16_t getSingleValueToWrite() const;                        // w, req/res
    uint8_t getNumberOfBytesOfValuesToWrite() const;               // w, req
    uint8_t getNumberOfBytesOfReadValues() const;                  // r, res
    std::vector<uint8_t> getCoilValuesToWrite() const;             // w, req
    std::vector<uint16_t> getHoldingRegisterValuesToWrite() const; // w, req
    std::vector<uint8_t> getReadBitValues() const;                 // r, res
    std::vector<uint16_t> getReadRegisterValues() const;           // r, res

    std::vector<uint8_t> asByteVector();

    friend std::ostream& operator<<(std::ostream& os, const ModbusTcpMessageFrame& mbMsgFrame);

    static ModbusTcpMessageFrame fromByteVector(const std::vector<uint8_t>& byteVector);

private:
    std::vector<uint8_t> extractBitValues(int startByte) const;
    std::vector<uint16_t> extractRegisterValues(int startByte) const;
};

}
