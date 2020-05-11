#include "domain/framework/includes/LibModbusSlave.hpp"

#include <iostream>

namespace Framework {

LibModbusSlave::LibModbusSlave()
    : m_slaveSocket(-1)
{}

bool LibModbusSlave::init(const std::string& ipAddr, const int port, const Gateway::ModbusDataMapping& mbMapping)
{
    // Modbus context
    initModbusContext(ipAddr, port);
    if (m_modbusContext == nullptr) {
#ifdef DEBUG
        std::cerr << "[LibModbusSlave] Unable to allocate libmodbus context\n";
#endif
        return false;
    }

    // Modbus mapping
    initModbusMapping(mbMapping);
    if (m_modbusMapping == nullptr) {
#ifdef DEBUG
        std::cerr << "[LibModbusSlave] Failed to allocate the mapping\n";
#endif
        return false;
    }

#ifdef DEBUG
    modbus_set_debug(m_modbusContext.get(), true);
#endif

    return true;
}

void LibModbusSlave::run()
{
#ifdef DEBUG
    std::cout << "[LibModbusSlave] Listen for a incoming connection\n";
#endif

    // wait for incoming connection (only 1 allowed)
    m_slaveSocket = modbus_tcp_listen(m_modbusContext.get(), 1);
    modbus_tcp_accept(m_modbusContext.get(), &m_slaveSocket);

#ifdef DEBUG
    std::cout << "[LibModbusSlave] Accepted incoming connection\n";
#endif

    int rc = 0;
    auto modbusRequest = std::make_unique<uint8_t[]>(MODBUS_TCP_MAX_ADU_LENGTH);

    // infinite request loop
    for (;;) {
        do {
            rc = modbus_receive(m_modbusContext.get(), modbusRequest.get());
        } while (rc == 0); // 0:= indication request ignored

        if (rc == -1) {
            std::cerr << "[LibModbusSlave] Failed to receive incoming request\n";
            break;
        }

        // TODO(Markus2101, 10.05.2020): forward request to ModbusGateway here;
        //                               for now, request is simply returned
        rc = modbus_reply(m_modbusContext.get(), modbusRequest.get(), rc, m_modbusMapping.get());
        if (rc == -1) {
            std::cerr << "[LibModbusSlave] Failed to return response\n";
            break;
        }
    }

    // stop connection if loop was left
    modbus_close(m_modbusContext.get());
}

void LibModbusSlave::initModbusContext(const std::string& ipAddr, const int port)
{
    // clang-format off
    m_modbusContext = std::move(std::unique_ptr<modbus_t, std::function<void(modbus_t*)>>(modbus_new_tcp(ipAddr.c_str(), port),
        [this](modbus_t* mbCtx) {
            // use custom-deleter provided by libmodbus
            modbus_free(mbCtx);
        }));
    // clang-format on
}

void LibModbusSlave::initModbusMapping(const Gateway::ModbusDataMapping& mbMapping)
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
}

}
