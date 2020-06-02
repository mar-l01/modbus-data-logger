#include "domain/gateway/includes/ModbusSlaveController.hpp"

#include <vector>

namespace Gateway {

ModbusSlaveController::ModbusSlaveController(const std::shared_ptr<ModbusSlave>& mbSlave,
                                             const std::shared_ptr<ModbusRequestController>& mbReqCtrl,
                                             const Entity::ModbusDataMapping& mbDataMapping, const std::string& ipAddr,
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
    ModbusReceiveStatus mbRecStatus = ModbusReceiveStatus::OK;
    auto modbusRequest = std::make_shared<Entity::ModbusTcpRequest>();

    // infinite request loop
    for (;;) {
        do {
            mbRecStatus = m_modbusSlave->receive(modbusRequest);
        } while (mbRecStatus == ModbusReceiveStatus::IGNORED);

        // error in receiving request
        if (mbRecStatus == ModbusReceiveStatus::FAILED) {
            std::cerr << "[ModbusSlaveController] Failed to receive incoming request\n";
            break;
        }

        // Modbus master closed connection
        if (mbRecStatus == ModbusReceiveStatus::CONNECTION_CLOSED_BY_MASTER) {
            std::cerr << "[ModbusSlaveController] Modbus master closed connection\n";
            break;
        }

        // check if function code is supported, if so reply with respective exception
        if (not modbusRequest->isFunctionCodeSupported()) {
            // TODO (Markus2101, 30.05.2020): return error-code here
            mbRecStatus = m_modbusSlave->replyException(Entity::ModbusExceptionCode::ILLEGAL_FUNCTION);
        } else {
            // forward Modbus request via gateway to external Modbus slave and receive response
            auto modbusResponse = m_modbusRequestController->forwardModbusRequestAndWaitForResponse(modbusRequest);

            mbRecStatus = m_modbusSlave->reply(modbusResponse);
        }

        // error in replying response
        if (mbRecStatus == ModbusReceiveStatus::FAILED) {
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
