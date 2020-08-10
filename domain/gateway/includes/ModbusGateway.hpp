#pragma once

#include "domain/gateway/interfaces/ModbusRequestController.hpp"

namespace Logging {
// forward declaration
class DataLogger;
}

namespace Gateway {
// forward declaration
class ModbusResponseController;

/**
 * @brief This class represents a Modbus gateway. It is responsible for forwarding incoming Modbus request
 * to the external Modbus slave instance. The returned of this slave will again be forwarded via this gateway
 * to the external Modbus master, which triggered the former request.
 * It inherits from @ref ModbusRequestController, which is capable of handling incoming Modbus requestes, i.e.
 * it acts as Modbus master.
 */
class ModbusGateway : public ModbusRequestController
{
public:
    /**
     * @brief Construct a new ModbusGateway object. The provided controller instance will internally used as
     * Modbus master.
     *
     * @param mbResponseController A Modbus master instance (derived from @ref ModbusResponseController),
     * which will be used to forward incoming requests.
     * @param dataLogger A data logger instance (derived from @ref Logging::DataLogger), which handles the logging of
     * the Modbus communication
     */
    ModbusGateway(const std::shared_ptr<ModbusResponseController>& mbResponseController,
                  const std::shared_ptr<Logging::DataLogger>& dataLogger);

    /**
     * @see ModbusRequestController::forwardModbusRequestAndWaitForResponse
     */
    std::shared_ptr<Entity::ModbusTcpResponse> forwardModbusRequestAndWaitForResponse(
      std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest) override;

private:
    std::shared_ptr<ModbusResponseController> m_modbusResponseController;
    std::shared_ptr<Logging::DataLogger> m_dataLogger;
};

}
