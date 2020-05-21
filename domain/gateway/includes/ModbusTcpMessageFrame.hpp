#include <cstdint>
#include <iostream>
#include <vector>

namespace Gateway {

class ModbusTcpMessageFrame
{
public:
    uint16_t transactionIdentifier;
    uint16_t protocolIdentifier;
    uint16_t lengthField;
    uint8_t unitIdentifier;
    uint8_t functionCode;
    std::vector<uint8_t> dataBytes;

    friend std::ostream& operator<<(std::ostream& os, const ModbusTcpMessageFrame& mbMsgFrame);
};

}
