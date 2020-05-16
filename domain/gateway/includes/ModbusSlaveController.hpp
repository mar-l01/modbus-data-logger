#pragma once

#include "domain/gateway/interfaces/ModbusRequestController.hpp"
#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include <memory>

namespace Gateway {

namespace ModbusConstants {
constexpr const int NUMBER_CONNECTIONS_IN = 1;
constexpr const int MODBUS_TCP_REQUEST_LENGTH_MAX = 260;
}

class ModbusSlaveController
{
public:
    ModbusSlaveController(const std::shared_ptr<ModbusSlave>& mbSlave,
                          const std::shared_ptr<ModbusRequestController>& mbReqCtrl,
                          const ModbusDataMapping& mbDataMapping, const std::string& ipAddr, const int port);

    void waitForIncomingConnection();
    void run();
    void closeConnection();

private:
    std::shared_ptr<ModbusSlave> m_modbusSlave;
    std::shared_ptr<ModbusRequestController> m_modbusRequestController;
    ModbusDataMapping m_modbusDataMapping;
    std::string m_ipAddress;
    int m_port;
    int m_socket;
};

}
