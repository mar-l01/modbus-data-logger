#include "domain/gateway/includes/ModbusTcpMessageFrame.hpp"

#include <iomanip>

namespace Gateway {

ModbusTcpMessageFrame::ModbusTcpMessageFrame() {}

uint16_t ModbusTcpMessageFrame::getStartAddress() const
{
    // allowed FCs: all
    // start address := bytes 0 and 1 of data-bytes
    return static_cast<uint16_t>(dataBytes[0] << 8) + dataBytes[1];
}

uint16_t ModbusTcpMessageFrame::getNumberOfValuesToReadOrWrite() const
{
    // allowed FCs: 0x01, 0x02, 0x03, 0x04, 0x0f, 0x10
    // number of values to read/write := bytes 2 and 3 of data-bytes
    return static_cast<uint16_t>(dataBytes[2] << 8) + dataBytes[3];
}

uint16_t ModbusTcpMessageFrame::getSingleValueToWrite() const
{
    // allowed FCs: 0x05, 0x06
    // single value to write (coil or holding register) := bytes 2 and 3 of data-bytes
    return static_cast<uint16_t>(dataBytes[2] << 8) + dataBytes[3];
}

uint8_t ModbusTcpMessageFrame::getNumberOfBytesOfValuesToWrite() const
{
    // allowed FCs: 0x0f, 0x10
    // number of bytes to write (multiple coils or registers) := byte 4 of data-bytes
    return dataBytes[4];
}

std::vector<uint8_t> ModbusTcpMessageFrame::getCoilValuesToWrite() const
{
    // allowed FC: 0x0f (start byte 5)
    return extractBitValues(ModbusByteOffset::START_BYTE_WRITE_VALUES);
}

std::vector<uint16_t> ModbusTcpMessageFrame::getHoldingRegisterValuesToWrite() const
{
    // allowed FC: 0x10 (start byte 5)
    return extractRegisterValues(ModbusByteOffset::START_BYTE_WRITE_VALUES);
}

uint8_t ModbusTcpMessageFrame::getNumberOfBytesOfReadValues() const
{
    // allowed FCs: 0x01, 0x02, 0x03, 0x04
    // number of bytes to read (multiple bits or registers) := byte 0 of data-bytes
    return dataBytes[0];
}

std::vector<uint8_t> ModbusTcpMessageFrame::getReadBitValues() const
{
    // allowed FC: 0x01, 0x02 (start byte 1)
    return extractBitValues(ModbusByteOffset::START_BYTE_READ_VALUES);
}

std::vector<uint16_t> ModbusTcpMessageFrame::getReadRegisterValues() const
{
    // allowed FC: 0x03, 0x04 (start byte 1)
    return extractRegisterValues(ModbusByteOffset::START_BYTE_READ_VALUES);
}

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

std::vector<uint8_t> ModbusTcpMessageFrame::extractBitValues(int startByte) const
{
    std::vector<uint8_t> bitValues;

    // number of bits
    auto nbBitValues = getNumberOfValuesToReadOrWrite();

    // each bit represents one coil value, first requested coil is at LSB of first byte,
    // MSBs of last byte are stuffed with 0 if they do not represent a coil
    int currentBit = 0;
    int currentByte = startByte;

    for (int i = 0; i < nbBitValues; ++i) {
        bitValues.emplace_back(((dataBytes[currentByte] >> currentBit) & 1) == 1 ? 1 : 0);

        // jump to next byte and reset bit counter
        if (currentBit == 7) {
            currentBit = 0;
            ++currentByte;
        }
    }

    return bitValues;
}

std::vector<uint16_t> ModbusTcpMessageFrame::extractRegisterValues(int startByte) const
{
    std::vector<uint16_t> registerValues;

    // number of registers
    auto nbRegisterValues = getNumberOfValuesToReadOrWrite();

    // each 2-bytes represent a single register
    for (int i = 0; i < nbRegisterValues; ++i) {
        registerValues.emplace_back(static_cast<uint16_t>(dataBytes[startByte + i] << 8) +
                                    dataBytes[startByte + i + 1]);
    }

    return registerValues;
}

}
