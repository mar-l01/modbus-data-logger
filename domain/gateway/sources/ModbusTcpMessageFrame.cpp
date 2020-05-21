#include "domain/gateway/includes/ModbusTcpMessageFrame.hpp"

namespace Gateway {

ModbusTcpMessageFrame::ModbusTcpMessageFrame() {}

std::vector<uint8_t> ModbusTcpMessageFrame::asByteVector()
{
    std::vector<uint8_t> convByteVector;

    convByteVector.emplace_back(transactionIdentifier >> 8);
    convByteVector.emplace_back(transactionIdentifier & 0x00ff);
    convByteVector.emplace_back(protocolIdentifier >> 8);
    convByteVector.emplace_back(protocolIdentifier & 0x00ff);
    convByteVector.emplace_back(lengthField >> 8);
    convByteVector.emplace_back(lengthField & 0x00ff);
    convByteVector.emplace_back(unitIdentifier);
    convByteVector.emplace_back(functionCode);

    for (const auto& dataByte : dataBytes) {
        convByteVector.emplace_back(dataByte);
    }

    return convByteVector;
}

ModbusTcpMessageFrame ModbusTcpMessageFrame::fromByteVector(const std::vector<uint8_t>& byteVector)
{
    ModbusTcpMessageFrame mbMsgFrame;

    mbMsgFrame.transactionIdentifier = (byteVector[0] << 8) + byteVector[1];
    mbMsgFrame.protocolIdentifier = (byteVector[2] << 8) + byteVector[3];
    mbMsgFrame.lengthField = (byteVector[4] << 8) + byteVector[5];
    mbMsgFrame.unitIdentifier = byteVector[6];
    mbMsgFrame.unitIdentifier = byteVector[7];

    for (int i = 8; i < byteVector.size(); ++i) {
        mbMsgFrame.dataBytes[i - 8] = byteVector[i];
    }

    return mbMsgFrame;
}

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
