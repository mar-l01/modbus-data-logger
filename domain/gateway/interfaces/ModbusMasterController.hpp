#pragma once

namespace Gateway {

/**
 * @brief This interface provides basic methods to start and close a connection to an external Modbus slave.
 */
class ModbusMasterController
{
public:
    /**
     * @brief Establish a connection to an external Modbus slave.
     */
    virtual void connect() = 0;

    /**
     * @brief Close connection to the external Modbus slave.
     */
    virtual void disconnect() = 0;

    /**
     * @brief Determines if the connection is up, i.e. the internal master is connected to the external slave
     *
     * @return true, if the master is still connected, false otw.
     */
    virtual bool isConnectedToExternalSlave() = 0;
};

}
