#include "domain/gateway/includes/ModbusGateway.hpp"

#include "domain/gateway/interfaces/ModbusResponseController.hpp"
#include "domain/logging/interfaces/DataLogger.hpp"

namespace Gateway {

ModbusGateway::ModbusGateway(const std::shared_ptr<ModbusResponseController>& mbResponseController,
                             const std::shared_ptr<Logging::DataLogger>& dataLogger)
    : m_modbusResponseController(mbResponseController)
    , m_dataLogger(dataLogger)
{}

std::shared_ptr<Entity::ModbusTcpResponse> ModbusGateway::forwardModbusRequestAndWaitForResponse(
  std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest)
{
    m_dataLogger->logModbusRequest(mbRequest);

    // forward Modbus request to controller which executes this request on external Modbus slave
    auto mbResponse = m_modbusResponseController->getExternalModbusSlaveResponse(mbRequest);

    m_dataLogger->logModbusResponse(mbResponse);

    return mbResponse;
}

}
