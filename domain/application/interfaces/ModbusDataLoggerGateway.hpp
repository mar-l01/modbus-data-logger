#pragma once

namespace Application {

/**
 * @brief This interface will be provided to an optional GUI-application. It will be able to
 * start/stop the Modbus gateway communication as well as the logging.
 */
class ModbusDataLoggerGateway
{
public:
    /**
     * @brief Start Modbus communication via the Modbus gateway
     */
    virtual void startModbusCommunication() = 0;

    /**
     * @brief Stop Modbus communication via the Modbus gateway
     */
    virtual void stopModbusCommunication() = 0;

    /**
     * @brief Start logging the Modbus communication
     */
    virtual void startLogger();

    /**
     * @brief Stop logging the Modbus communication
     */
    virtual void stopLogger();
};

}
