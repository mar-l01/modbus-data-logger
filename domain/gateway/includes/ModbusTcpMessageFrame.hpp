#include <cstdint>
#include <iostream>
#include <vector>

namespace Gateway {

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

    std::vector<uint8_t> asByteVector();

    friend std::ostream& operator<<(std::ostream& os, const ModbusTcpMessageFrame& mbMsgFrame);

    static ModbusTcpMessageFrame fromByteVector(const std::vector<uint8_t>& byteVector);
};

}
