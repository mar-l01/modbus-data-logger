#include "domain/gateway/includes/ModbusMasterController.hpp"

namespace Gateway {

ModbusMasterController::ModbusMasterController(const std::shared_ptr<ModbusMaster>& mbMaster, const std::string& ipAddr,
                                               const int port)
    : m_modbusMaster(mbMaster)
    , m_ipAddress(ipAddr)
    , m_port(port)
{}

void ModbusMasterController::connect()
{
    m_modbusMaster->connect(m_ipAddress, m_port);
}

std::shared_ptr<Entity::ModbusTcpResponse> ModbusMasterController::getExternalModbusSlaveResponse(
  std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest)
{
    // TODO(Markus2101, 24.05.2020): call respective method of Modbus master instance to obtain the reply
    return std::make_shared<Entity::ModbusTcpResponse>();
}

void ModbusMasterController::closeConnection()
{
    m_modbusMaster->close();
}

}
