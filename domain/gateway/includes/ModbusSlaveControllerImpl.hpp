#pragma once

#include "domain/gateway/interfaces/ModbusRequestController.hpp"
#include "domain/gateway/interfaces/ModbusSlave.hpp"
#include "domain/gateway/interfaces/ModbusSlaveController.hpp"
#include "domain/utility/interfaces/Timer.hpp"

#include <memory>

namespace Gateway {

/**
 * @brief Pre-set constant values used within a Modbus communication
 */
namespace ModbusConstants {
constexpr const int NUMBER_CONNECTIONS_IN = 1;
constexpr const int MODBUS_TCP_REQUEST_LENGTH_MAX = 260;
}

/**
 * @brief This class is internally used to control the @ref ModbusSlave object, e.g. a @ref Framework::LibModbusSlave.
 * It provides methods to
 * - wait for an incoming connection (if so, a connection gets established),
 * - run the Modbus slave functionality (receive request, process it, reply) and
 * - close established connection.
 * It implements the @ref ModbusSlaveController interface to establish and close a connection.
 */
class ModbusSlaveControllerImpl : public ModbusSlaveController
{
public:
    /**
     * @brief Construct a new ModbusSlaveController object, which controls the @ref ModbusSlave instance,
     * representing the internal Modbus slave.
     *
     * @param mbSlave An instance derived @ref ModbusSlave interface, e.g. @ref Framework::LibModbusSlave.
     * @param mbReqCtrl An instance derived from @ref ModbusRequestController, e.g. @ref  ModbusGateway.
     * @param timerInstance An @ref Utility::Timer instance used to control timeouts.
     * @param mbDataMapping A @ref Entity::ModbusDataMapping which holds information about all Modbus data-types.
     * @param ipAddr The IP-address the internal Modbus slave will listen to.
     * @param port The port-value the internal Modbus slave will listen on.
     */
    ModbusSlaveControllerImpl(const std::shared_ptr<ModbusSlave>& mbSlave,
                              const std::shared_ptr<ModbusRequestController>& mbReqCtrl,
                              const std::shared_ptr<Utility::Timer>& timerInstance,
                              const Entity::ModbusDataMapping& mbDataMapping, const std::string& ipAddr,
                              const int port);

    /**
     * @see ModbusSlaveController::waitForIncomingConnection
     */
    void waitForIncomingConnection() override;

    /**
     * @see ModbusSlaveController::run
     */
    void run() override;

    /**
     * @see ModbusSlaveController::disconnect
     */
    void disconnect() override;

    /**
     * @see ModbusSlaveController::isExternalMasterConnected
     */
    bool isExternalMasterConnected() override;

private:
    std::shared_ptr<ModbusSlave> m_modbusSlave;
    std::shared_ptr<ModbusRequestController> m_modbusRequestController;
    std::shared_ptr<Utility::Timer> m_timer;
    Entity::ModbusDataMapping m_modbusDataMapping;
    std::string m_ipAddress;
    int m_port;
    int m_socket;
};

}
