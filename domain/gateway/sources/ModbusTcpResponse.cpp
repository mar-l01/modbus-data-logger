#include "domain/gateway/includes/ModbusTcpResponse.hpp"

#include "domain/gateway/includes/ModbusTcpRequest.hpp"

namespace Gateway {

ModbusTcpResponse::ModbusTcpResponse() {}

ModbusTcpResponse::ModbusTcpResponse(const std::vector<uint8_t>& mbTcpRes,
                                     const std::shared_ptr<ModbusTcpRequest>& mbTcpReq)
    : m_accordingModbusTcpRequest(mbTcpReq)
{
    initFromByteVector(mbTcpRes);
}

uint8_t ModbusTcpResponse::getNumberOfBytesOfReadValues() const
{
    // allowed FCs: 0x01, 0x02, 0x03, 0x04
    // number of bytes to read (multiple bits or registers) := byte 0 of data-bytes
    return dataBytes[0];
}

std::vector<uint8_t> ModbusTcpResponse::getReadBitValues() const
{
    return extractBitValues(ModbusByteOffset::START_BYTE_READ_VALUES,
                            m_accordingModbusTcpRequest->getNumberOfValuesToReadOrWrite());
}

std::vector<uint16_t> ModbusTcpResponse::getReadRegisterValues() const
{
    return extractRegisterValues(ModbusByteOffset::START_BYTE_READ_VALUES,
                                 m_accordingModbusTcpRequest->getNumberOfValuesToReadOrWrite());
}

}
