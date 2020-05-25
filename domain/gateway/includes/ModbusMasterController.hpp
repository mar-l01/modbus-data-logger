#pragma once

#include "domain/gateway/interfaces/ModbusMaster.hpp"
#include "domain/gateway/interfaces/ModbusResponseController.hpp"

#include <memory>

namespace Gateway {

enum class ModbusFunctionCode : uint8_t
{
    // read FCs
    READ_COIL_VALUES = 0x01,
    READ_DISCRETE_INPUT_VALUES,
    READ_HOLDING_REGISTER_VALUES,
    READ_INPUT_REGISTER_VALUES,

    // write FCs
    WRITE_SINGLE_COIL_VALUE,
    WRITE_SINGLE_HOLDING_REGISTER_VALUE,
    WRITE_MULTIPLE_COIL_VALUES = 0x0f,
    WRITE_MULTIPLE_HOLDING_REGISTER_VALUES
};

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
