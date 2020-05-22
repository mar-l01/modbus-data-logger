#include "domain/gateway/includes/ModbusTcpMessageFrame.hpp"

#include <iomanip>

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

    mbMsgFrame.transactionIdentifier = static_cast<uint16_t>(byteVector[0] << 8) + byteVector[1];
    mbMsgFrame.protocolIdentifier = static_cast<uint16_t>(byteVector[2] << 8) + byteVector[3];
    mbMsgFrame.lengthField = static_cast<uint16_t>(byteVector[4] << 8) + byteVector[5];
    mbMsgFrame.unitIdentifier = byteVector[6];
    mbMsgFrame.functionCode = byteVector[7];

    // only use the required data-bytes specified by the length field (-2 := unit-id and fc)
    for (int i = 0; i < static_cast<int>(mbMsgFrame.lengthField) - 2; ++i) {
        mbMsgFrame.dataBytes.emplace_back(byteVector[i + 8]);
    }

    return mbMsgFrame;
}

std::ostream& operator<<(std::ostream& os, const ModbusTcpMessageFrame& mbMsgFrame)
{
    os << "ModbusTcpMessageFrame: "
       << "[Transaction Identifier] 0x" << std::hex << std::setw(4) << std::setfill('0')
       << mbMsgFrame.transactionIdentifier << '\n'
       << "[Protocol Identifier] 0x" << std::hex << std::setw(4) << std::setfill('0') << mbMsgFrame.protocolIdentifier
       << '\n'
       << "[Length Field] 0x" << std::hex << std::setw(4) << std::setfill('0') << mbMsgFrame.lengthField << '\n'
       << "[Unit Identifier] 0x" << std::hex << std::setw(2) << std::setfill('0')
       << static_cast<int>(mbMsgFrame.unitIdentifier) << '\n'
       << "[Function Code] 0x" << std::hex << std::setw(2) << std::setfill('0')
       << static_cast<int>(mbMsgFrame.functionCode) << '\n'
       << "[Data Bytes] ";

    for (const auto& dataByte : mbMsgFrame.dataBytes) {
        os << "0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(dataByte) << " | ";
    }

    return os;
}

}
