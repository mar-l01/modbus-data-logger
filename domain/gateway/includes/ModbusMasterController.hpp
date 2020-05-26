#pragma once

#include "domain/gateway/interfaces/ModbusMaster.hpp"
#include "domain/gateway/interfaces/ModbusResponseController.hpp"

#include <memory>

namespace Gateway {

class ModbusMasterController : public ModbusResponseController
{
public:
    ModbusMasterController(const std::shared_ptr<ModbusMaster>& mbMaster, const std::string& ipAddr, const int port);

    void connect();

    std::shared_ptr<Entity::ModbusTcpResponse> getExternalModbusSlaveResponse(
      std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest) override;

    void closeConnection();

private:
    std::shared_ptr<ModbusMaster> m_modbusMaster;
    std::string m_ipAddress;
    int m_port;

    std::shared_ptr<Entity::ModbusTcpResponse> callModbusMasterMethod(
      std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest);
};

}
