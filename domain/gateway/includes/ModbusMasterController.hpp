#pragma once

#include "domain/gateway/interfaces/ModbusMaster.hpp"
#include "domain/gateway/interfaces/ModbusResponseController.hpp"

#include <memory>

namespace Gateway {

/**
 * @brief This class is internally used as Modbus master. It implements the @ref ModbusResponseController interface.
 */
class ModbusMasterController : public ModbusResponseController
{
public:
    /**
     * @brief Construct a new ModbusMasterController object.
     *
     * @param mbMaster A @ref ModbusMaster instance, e.g. @ref Framework::LibModbusMaster
     * @param ipAddr The IP-address of the external Modbus slave, which above master instance should connect to.
     * @param port The port value of the external Modbus slave, which above master instance should connect to.
     *  */
    ModbusMasterController(const std::shared_ptr<ModbusMaster>& mbMaster, const std::string& ipAddr, const int port);

    /**
     * @brief If called, it tries to establish a connection to the external Modbus slave.
     */
    void connect();

    /**
     * @brief Set given timeout value, as Modbus message timeout.
     *
     * @param timeoutInMs Timeout value to set, provided in milliseconds.
     */
    void setTimeout(const uint16_t timeoutInMs);

    /**
     * @see ModbusResponseController::getExternalModbusSlaveResponse
     */
    std::shared_ptr<Entity::ModbusTcpResponse> getExternalModbusSlaveResponse(
      std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest) override;

    /**
     * @brief Close the established connection to an external Modbus slave.
     */
    void closeConnection();

private:
    std::shared_ptr<ModbusMaster> m_modbusMaster;
    std::string m_ipAddress;
    int m_port;

    std::shared_ptr<Entity::ModbusTcpResponse> callModbusMasterMethod(
      std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest);
    void tryReconnecting();
};

}
