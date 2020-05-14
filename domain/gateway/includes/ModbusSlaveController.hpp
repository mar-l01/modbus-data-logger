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
    ModbusSlaveController(std::unique_ptr<ModbusSlave> mbSlave, const ModbusDataMapping& mbDataMapping,
                          const std::string& ipAddr, const int port);

    void connect();
    void run();
    void closeConnection();

private:
    std::unique_ptr<ModbusSlave> m_modbusSlave;
    ModbusDataMapping m_modbusDataMapping;
    std::string m_ipAddress;
    int m_port;
    int m_socket;
};

}
