#pragma once

namespace Gateway {

/**
 * @brief This interface provides a method to close a connection to an external Modbus master.
 */
class ModbusSlaveController
{
public:
    /**
     * @brief Let a @ref ModbusSlave instance listen for an incoming connection of an external Modbus master.
     * A connection is established if the external Modbus master was able to connect.
     */
    virtual void waitForIncomingConnection() = 0;

    /**
     * @brief Run the basic Modbus slave functionality in an infinite:
     * - wait for an incoming Modbus request
     * - process this request
     * - reply with a Modbus response (can also be a Modbus exception)
     *
     * If an error occurrs (which can not be answered by a Modbus exception), the loop gets stopped
     * and the connection closed.
     */
    virtual void run() = 0;

    /**
     * @brief Close connection to the external Modbus master.
     */
    virtual void disconnect() = 0;
};

}
