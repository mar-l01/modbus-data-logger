#include "domain/gateway/includes/ModbusSlaveController.hpp"

#include <vector>

namespace Gateway {

ModbusSlaveController::ModbusSlaveController(const std::shared_ptr<ModbusSlave>& mbSlave,
                                             const std::shared_ptr<ModbusRequestController>& mbReqCtrl,
                                             const ModbusDataMapping& mbDataMapping, const std::string& ipAddr,
                                             const int port)
    : m_modbusSlave(mbSlave)
    , m_modbusRequestController(mbReqCtrl)
    , m_modbusDataMapping(mbDataMapping)
    , m_ipAddress(ipAddr)
    , m_port(port)
    , m_socket(-1)
{
    // set Modbus data mapping
    m_modbusSlave->setModbusDataMapping(m_modbusDataMapping);
}

void ModbusSlaveController::waitForIncomingConnection()
{
    // set up Modbus slave connection
    m_modbusSlave->bind(m_ipAddress, m_port);
    m_socket = m_modbusSlave->listen(ModbusConstants::NUMBER_CONNECTIONS_IN);
    m_modbusSlave->accept(m_socket);
}

void ModbusSlaveController::run()
{
    int reqLen = 0;
    ModbusTcpMessageFrame modbusRequest;

    // infinite request loop
    for (;;) {
        do {
            reqLen = m_modbusSlave->receive(modbusRequest);
        } while (reqLen == 0); // 0:= indication request ignored

        // error in receiving request
        if (reqLen == -1) {
            std::cerr << "[ModbusSlaveController] Failed to receive incoming request\n";
            break;
        }

        // forward Modbus request via gateway to external Modbus slave and receive response
        auto modbusResponse = m_modbusRequestController->forwardModbusRequestAndWaitForResponse(modbusRequest);

        reqLen = m_modbusSlave->reply(modbusResponse);

        // error in replying response
        if (reqLen == -1) {
            std::cerr << "[ModbusSlaveController] Failed to return response\n";
            break;
        }
    }
}

void ModbusSlaveController::closeConnection()
{
    m_modbusSlave->close();
}

}
