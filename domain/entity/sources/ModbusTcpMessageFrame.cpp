#include "domain/entity/includes/ModbusTcpMessageFrame.hpp"

#include "domain/entity/includes/ModbusTcpConstants.hpp"

#include <iomanip>

namespace Entity {

ModbusTcpMessageFrame::ModbusTcpMessageFrame() {}

uint16_t ModbusTcpMessageFrame::getStartAddress() const
{
    // allowed FCs: all
    // start address := bytes 0 and 1 of data-bytes
    return static_cast<uint16_t>(dataBytes[ModbusDataByteOffset::START_ADDRESS] << 8) +
           dataBytes[ModbusDataByteOffset::START_ADDRESS + 1];
}

uint16_t ModbusTcpMessageFrame::getNumberOfValuesToReadOrWrite() const
{
    // allowed FCs: 0x01, 0x02, 0x03, 0x04, 0x0f, 0x10
    // number of values to read/write := bytes 2 and 3 of data-bytes
    return static_cast<uint16_t>(dataBytes[ModbusDataByteOffset::NUMBER_OF_VALUES_TO_READ_OR_WRITE] << 8) +
           dataBytes[ModbusDataByteOffset::NUMBER_OF_VALUES_TO_READ_OR_WRITE + 1];
}

uint16_t ModbusTcpMessageFrame::getSingleValueToWrite() const
{
    // allowed FCs: 0x05, 0x06
    // single value to write (coil or holding register) := bytes 2 and 3 of data-bytes
    return static_cast<uint16_t>(dataBytes[ModbusDataByteOffset::SINGLE_VALUE_TO_WRITE] << 8) +
           dataBytes[ModbusDataByteOffset::SINGLE_VALUE_TO_WRITE + 1];
}

std::vector<uint8_t> ModbusTcpMessageFrame::extractBitValues(int startByte, int nbBitValues) const
{
    std::vector<uint8_t> bitValues;

    // each bit represents one coil value, first requested coil is at LSB of first byte,
    // MSBs of last byte are stuffed with 0 if they do not represent a coil
    int currentBit = 0;
    int currentByte = startByte;

    for (int i = 0; i < nbBitValues; ++i) {
        bitValues.emplace_back((dataBytes[currentByte] >> currentBit) & 1);

        // jump to next byte and reset bit counter or move to next bit
        if (currentBit == 7) {
            currentBit = 0;
            ++currentByte;
        } else {
            ++currentBit;
        }
    }

    return bitValues;
}

std::vector<uint16_t> ModbusTcpMessageFrame::extractRegisterValues(int startByte, int nbRegisterValues) const
{
    std::vector<uint16_t> registerValues;

    // each 2-bytes represent a single register
    for (int i = 0; i < nbRegisterValues * 2; i += 2) {
        registerValues.emplace_back(static_cast<uint16_t>(dataBytes[startByte + i] << 8) +
                                    dataBytes[startByte + i + 1]);
    }

    return registerValues;
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

void ModbusTcpMessageFrame::initFromByteVector(const std::vector<uint8_t>& byteVector)
{
    transactionIdentifier = static_cast<uint16_t>(byteVector[ModbusMessageFrameByte::TRANSACTION_ID] << 8) +
                            byteVector[ModbusMessageFrameByte::TRANSACTION_ID + 1];
    protocolIdentifier = static_cast<uint16_t>(byteVector[ModbusMessageFrameByte::PROTOCOL_ID] << 8) +
                         byteVector[ModbusMessageFrameByte::PROTOCOL_ID + 1];
    lengthField = static_cast<uint16_t>(byteVector[ModbusMessageFrameByte::LENGTH_FIELD] << 8) +
                  byteVector[ModbusMessageFrameByte::LENGTH_FIELD + 1];
    unitIdentifier = byteVector[ModbusMessageFrameByte::UNIT_ID];
    functionCode = byteVector[ModbusMessageFrameByte::FUNCTION_CODE];

    // only use the required data-bytes specified by the length field (-2 := unit-id and fc)
    for (int i = 0; i < static_cast<int>(lengthField) - 2; ++i) {
        dataBytes.emplace_back(byteVector[i + 8]);
    }
}

bool operator==(const ModbusTcpMessageFrame& mbMsgFrameA, const ModbusTcpMessageFrame& mbMsgFrameB)
{
    return mbMsgFrameA.transactionIdentifier == mbMsgFrameB.transactionIdentifier &&
           mbMsgFrameA.protocolIdentifier == mbMsgFrameB.protocolIdentifier &&
           mbMsgFrameA.lengthField == mbMsgFrameB.lengthField &&
           mbMsgFrameA.unitIdentifier == mbMsgFrameB.unitIdentifier &&
           mbMsgFrameA.functionCode == mbMsgFrameB.functionCode && mbMsgFrameA.dataBytes == mbMsgFrameB.dataBytes;
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
