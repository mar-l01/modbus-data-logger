#include "domain/gateway/includes/ModbusSlaveController.hpp"

#include <unistd.h>
#include <vector>

namespace Gateway {

ModbusSlaveController::ModbusSlaveController(const std::shared_ptr<ModbusSlave>& mbSlave,
                                             const std::shared_ptr<ModbusRequestController>& mbReqCtrl,
                                             const std::shared_ptr<Utility::Timer>& timerInstance,
                                             const Entity::ModbusDataMapping& mbDataMapping, const std::string& ipAddr,
                                             const int port)
    : m_modbusSlave(mbSlave)
    , m_modbusRequestController(mbReqCtrl)
    , m_timer(timerInstance)
    , m_modbusDataMapping(mbDataMapping)
    , m_ipAddress(ipAddr)
    , m_port(port)
    , m_socket(-1)
{
    // set Modbus data mapping
    m_modbusSlave->setModbusDataMapping(m_modbusDataMapping);

    // bind slave to given ip address and port
    m_modbusSlave->bind(m_ipAddress, m_port);
}

void ModbusSlaveController::waitForIncomingConnection()
{
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

            // reset timer on each newly received Modbus request
            m_timer->restart();
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
            mbRecStatus = m_modbusSlave->replyException(Entity::ModbusExceptionCode::ILLEGAL_FUNCTION);
        } else {
            // forward Modbus request via gateway to external Modbus slave and receive response
            auto modbusResponse = m_modbusRequestController->forwardModbusRequestAndWaitForResponse(modbusRequest);

            // handle timeout of internal Modbus master to external Modbus slave
            if (modbusResponse->getModbusOperationStatus() == Entity::ModbusOperationStatus::TIMEOUT) {
                mbRecStatus =
                  m_modbusSlave->replyException(Entity::ModbusExceptionCode::GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND);
            } else {
                mbRecStatus = m_modbusSlave->reply(modbusResponse);
            }
        }

        // error in replying response
        if (mbRecStatus == ModbusReceiveStatus::FAILED) {
            std::cerr << "[ModbusSlaveController] Failed to return response\n";
            break;
        }
    }

    // close socket after connection (necessary to close file descriptor)
    if (close(m_socket) == 0) {
        // reset socket value
        m_socket = -1;
    } else {
        std::cerr << "[ModbusSlaveController] Failed to close socket!\n";
    }
}

void ModbusSlaveController::closeConnection()
{
    m_modbusSlave->close();
}

}
