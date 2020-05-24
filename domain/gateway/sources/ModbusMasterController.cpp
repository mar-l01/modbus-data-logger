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
    return std::make_shared<Entity::ModbusTcpResponse>();
}

}
