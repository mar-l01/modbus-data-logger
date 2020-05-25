#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include "domain/entity/includes/ModbusTcpRequest.hpp"

namespace Entity {

ModbusTcpResponse::ModbusTcpResponse() {}

ModbusTcpResponse::ModbusTcpResponse(const ModbusOperationStatus mbOpStatus)
    : m_operationStatus(mbOpStatus)
{}

uint8_t ModbusTcpResponse::getNumberOfBytesOfReadValues() const
{
    // allowed FCs: 0x01, 0x02, 0x03, 0x04
    // number of bytes to read (multiple bits or registers) := byte 0 of data-bytes
    return dataBytes[0];
}

std::vector<uint8_t> ModbusTcpResponse::getReadBitValues() const
{
    return std::get<std::vector<uint8_t>>(m_readValues);
}

std::vector<uint16_t> ModbusTcpResponse::getReadRegisterValues() const
{
    return std::get<std::vector<uint16_t>>(m_readValues);
}

void ModbusTcpResponse::setReadValues(const std::variant<std::vector<uint8_t>, std::vector<uint16_t>>& vals)
{
    m_readValues = vals;
}

}
