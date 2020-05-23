#include "domain/framework/includes/LibModbusSlave.hpp"

#include <iostream>

namespace Framework {

LibModbusSlave::LibModbusSlave()
    : m_messageLength(-1)
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

#ifdef DEBUG
    // error handling
    if (m_modbusContext == nullptr) {
        std::cerr << "[LibModbusSlave] Unable to allocate libmodbus context\n";
        std::cerr << "[LibModbusSlave] - IP: " << ipAddr << '\n';
        std::cerr << "[LibModbusSlave] - Port: " << port << '\n';
    } else {
        modbus_set_debug(m_modbusContext.get(), true);
    }
#endif
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

int LibModbusSlave::receive(Gateway::ModbusTcpRequest& request)
{
    auto mbRequest = std::vector<uint8_t>(MODBUS_TCP_MAX_ADU_LENGTH);

    m_messageLength = modbus_receive(m_modbusContext.get(), mbRequest.data());

    // save request (required for sending response)
    m_lastRequest = mbRequest;

    // convert byte vector into transferable object
    request = Gateway::ModbusTcpRequest(mbRequest);

    return m_messageLength;
}

int LibModbusSlave::reply(Gateway::ModbusTcpResponse& response)
{
    // TOOD(Markus2101, 13.05.2020): use response data from external slave and change
    //      mapping accordingly

    m_messageLength = modbus_reply(m_modbusContext.get(), m_lastRequest.data(), m_messageLength, m_modbusMapping.get());

    return m_messageLength;
}

void LibModbusSlave::close()
{
    modbus_close(m_modbusContext.get());
}

}
