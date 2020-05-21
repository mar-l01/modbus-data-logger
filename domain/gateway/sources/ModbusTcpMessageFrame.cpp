#include "domain/gateway/includes/ModbusTcpMessageFrame.hpp"

namespace Gateway {

std::ostream& operator<<(std::ostream& os, const ModbusTcpMessageFrame& mbMsgFrame)
{
    os << "ModbusTcpMessageFrame: "
       << "[Transaction Identifier] 0x" << std::hex << mbMsgFrame.transactionIdentifier << '\n'
       << "[Protocol Identifier] 0x" << std::hex << mbMsgFrame.protocolIdentifier << '\n'
       << "[Length Field] 0x" << std::hex << mbMsgFrame.lengthField << '\n'
       << "[Unit Identifier] 0x" << std::hex << mbMsgFrame.unitIdentifier << '\n'
       << "[Function Code] 0x" << std::hex << mbMsgFrame.functionCode << '\n'
       << "[Data Bytes] ";

    for (const auto& dataByte : mbMsgFrame.dataBytes) {
        os << "0x" << std::hex << dataByte << " | ";
    }

    return os;
}

}
