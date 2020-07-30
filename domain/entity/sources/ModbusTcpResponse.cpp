#include "domain/entity/includes/ModbusTcpResponse.hpp"


namespace Entity {

ModbusTcpResponse::ModbusTcpResponse() {}

ModbusTcpResponse::ModbusTcpResponse(const ModbusOperationStatus mbOpStatus)
    : m_operationStatus(mbOpStatus)
{}

ModbusOperationStatus ModbusTcpResponse::getModbusOperationStatus() const
{
    return m_operationStatus;
}

std::vector<uint8_t> ModbusTcpResponse::getReadBitValues() const
{
    auto readValues = std::vector<uint8_t>();

    try {
        readValues = std::get<std::vector<uint8_t>>(m_readValues);
    } catch (std::bad_variant_access&) {
        // TODO(Markus2101, 25.05.2020): strategy about error handling
    }

    return readValues;
}

std::vector<uint16_t> ModbusTcpResponse::getReadRegisterValues() const
{
    auto readValues = std::vector<uint16_t>();

    try {
        readValues = std::get<std::vector<uint16_t>>(m_readValues);
    } catch (std::bad_variant_access& ex) {
        // TODO(Markus2101, 25.05.2020): strategy about error handling
    }

    return readValues;
}

void ModbusTcpResponse::setReadValues(const ModbusReadValues& vals)
{
    m_readValues = vals;
}

bool operator==(const ModbusTcpResponse& mbResA, const ModbusTcpResponse& mbResB)
{
    return mbResA.m_operationStatus == mbResB.m_operationStatus && mbResA.m_readValues == mbResB.m_readValues;
}

}
