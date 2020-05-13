#include "domain/gateway/includes/ModbusSlaveController.hpp"

#include <vector>


namespace Gateway {

ModbusSlaveController::ModbusSlaveController(const std::shared_ptr<ModbusSlave>& mbSlave,
                                             const ModbusDataMapping& mbDataMapping, const std::string& ipAddr,
                                             const int port)
    : m_modbusSlave(mbSlave)
    , m_modbusDataMapping(mbDataMapping)
    , m_ipAddress(ipAddr)
    , m_port(port)
    , m_socket(-1)
{
    // set Modbus data mapping
    m_modbusSlave->setModbusDataMapping(m_modbusDataMapping);
}

void ModbusSlaveController::connect()
{
    // set up Modbus slave connection
    m_modbusSlave->bind(m_ipAddress, m_port);
    m_socket = m_modbusSlave->listen(ModbusConstants::NUMBER_CONNECTIONS_IN);
    m_modbusSlave->accept(m_socket);
}

void ModbusSlaveController::run()
{
    int reqLen = 0;
    auto modbusRequest = std::vector<uint8_t>(ModbusConstants::MODBUS_TCP_REQUEST_LENGTH_MAX);

    // infinite request loop
    for (;;) {
        do {
            reqLen = m_modbusSlave->receive(modbusRequest);
        } while (reqLen == 0); // 0:= indication request ignored

        // error in receiving request
        if (reqLen == -1) {
            std::cerr << "[ModbusSlaveHandler] Failed to receive incoming request\n";
            break;
        }

        // TODO(Markus2101, 10.05.2020): forward request to ModbusGateway here;
        //      for now, use dummy response
        auto modbusResponse = std::vector<uint8_t>(ModbusConstants::MODBUS_TCP_REQUEST_LENGTH_MAX);

        reqLen = m_modbusSlave->reply(modbusResponse);
        if (reqLen == -1) {
            std::cerr << "[ModbusSlaveHandler] Failed to return response\n";
            break;
        }
    }
}

void ModbusSlaveController::closeConnection()
{
    m_modbusSlave->close();
}

}
