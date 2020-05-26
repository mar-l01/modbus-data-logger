#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include <iostream>


namespace Entity {

ModbusTcpResponse::ModbusTcpResponse() {}

ModbusTcpResponse::ModbusTcpResponse(const ModbusOperationStatus mbOpStatus)
    : m_operationStatus(mbOpStatus)
{}

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

    std::cout << "[vec16] " << std::boolalpha << std::holds_alternative<std::vector<uint16_t>>(m_readValues) << '\n';
    std::cout << "[vec8] " << std::boolalpha << std::holds_alternative<std::vector<uint8_t>>(m_readValues) << '\n';

    try {
        readValues = std::get<std::vector<uint16_t>>(m_readValues);
    } catch (std::bad_variant_access& ex) {
        // TODO(Markus2101, 25.05.2020): strategy about error handling
        std::cout << ex.what() << '\n';
    }

    return readValues;
}

void ModbusTcpResponse::setReadValues(const ModbusReadValues& vals)
{
    m_readValues = vals;

    std::cout << "[vec16] " << std::boolalpha << std::holds_alternative<std::vector<uint16_t>>(m_readValues) << '\n';
    std::cout << "[vec8] " << std::boolalpha << std::holds_alternative<std::vector<uint8_t>>(m_readValues) << '\n';
}

}
