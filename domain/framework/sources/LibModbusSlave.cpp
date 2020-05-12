#include "domain/framework/includes/LibModbusSlave.hpp"

#include <iostream>

namespace Framework {

LibModbusSlave::LibModbusSlave()
    : m_requestLength(-1)
{}

void LibModbusSlave::setModbusDataMapping(const Gateway::ModbusDataMapping& mbMapping)
{
    // clang-format off
    m_modbusMapping = std::move(std::unique_ptr<modbus_mapping_t, std::function<void(modbus_mapping_t*)>>(modbus_mapping_new_start_address(
        mbMapping.startAddressCoils, mbMapping.nbCoils,
        mbMapping.startAddressDiscreteInputs, mbMapping.nbDiscreteInputs,
        mbMapping.startAddressHoldingRegisters, mbMapping.nbHoldingRegisters,
        mbMapping.startAddressInputRegisters, mbMapping.nbInputRegisters),
        [this](modbus_mapping_t* mbMapping){
            // use custom-deleter provided by libmodbus
            modbus_mapping_free(mbMapping);
        }));
    // clang-format on

    // error handling
    if (m_modbusMapping == nullptr) {
#ifdef DEBUG
        std::cerr << "[LibModbusSlave] Failed to allocate the mapping\n";
        std::cerr << "[LibModbusSlave] - Data-Mapping: " << mbMapping << '\n';
#endif
    }
}

void LibModbusSlave::bind(const std::string& ipAddr, const int port)
{
    // clang-format off
    m_modbusContext = std::move(std::unique_ptr<modbus_t, std::function<void(modbus_t*)>>(modbus_new_tcp(ipAddr.c_str(), port),
        [this](modbus_t* mbCtx) {
            // use custom-deleter provided by libmodbus
            modbus_free(mbCtx);
        }));
    // clang-format on

    // error handling
    if (m_modbusContext == nullptr) {
#ifdef DEBUG
        std::cerr << "[LibModbusSlave] Unable to allocate libmodbus context\n";
        std::cerr << "[LibModbusSlave] - IP: " << ipAddr << '\n';
        std::cerr << "[LibModbusSlave] - Port: " << port << '\n';
#endif
    }
}

int LibModbusSlave::listen(const int nbConns)
{
#ifdef DEBUG
    std::cout << "[LibModbusSlave] Listen for a incoming connection\n";
#endif

    auto slaveSocket = modbus_tcp_listen(m_modbusContext.get(), nbConns);

    return slaveSocket;
}

void LibModbusSlave::accept(int& socket)
{
    modbus_tcp_accept(m_modbusContext.get(), &socket);

#ifdef DEBUG
    std::cout << "[LibModbusSlave] Accepted incoming connection\n";
#endif
}

int LibModbusSlave::receive(std::vector<uint8_t>& request)
{
    m_requestLength = modbus_receive(m_modbusContext.get(), request.data());

    return m_requestLength;
}

int LibModbusSlave::reply(std::vector<uint8_t>& request)
{
    m_requestLength = modbus_reply(m_modbusContext.get(), request.data(), m_requestLength, m_modbusMapping.get());

    return m_requestLength;
}

void LibModbusSlave::close()
{
    modbus_close(m_modbusContext.get());
}

}
