#include "domain/entity/includes/ModbusTcpRequest.hpp"

namespace Entity {

ModbusTcpRequest::ModbusTcpRequest() {}

ModbusTcpRequest::ModbusTcpRequest(const std::vector<uint8_t>& mbTcpReq)
{
    initFromByteVector(mbTcpReq);
}

uint8_t ModbusTcpRequest::getNumberOfBytesOfValuesToWrite() const
{
    // allowed FCs: 0x0f, 0x10
    // number of bytes to write (multiple coils or registers) := byte 4 of data-bytes
    return dataBytes[4];
}

std::vector<uint8_t> ModbusTcpRequest::getCoilValuesToWrite() const
{
    // allowed FC: 0x0f (start byte 5)
    return extractBitValues(ModbusByteOffset::START_BYTE_WRITE_VALUES, getNumberOfValuesToReadOrWrite());
}

std::vector<uint16_t> ModbusTcpRequest::getHoldingRegisterValuesToWrite() const
{
    // allowed FC: 0x10 (start byte 5)
    return extractRegisterValues(ModbusByteOffset::START_BYTE_WRITE_VALUES, getNumberOfValuesToReadOrWrite());
}

}
