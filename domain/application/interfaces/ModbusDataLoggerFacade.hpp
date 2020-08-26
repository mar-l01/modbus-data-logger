#pragma once

namespace Application {

/**
 * @brief This interface represents the facade of the ModbusDataLogger application.
 * It allows to start/stop the Modbus gateway communication as well as the logging.
 */
class ModbusDataLoggerFacade
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
    virtual void startLogger() = 0;

    /**
     * @brief Stop logging the Modbus communication
     */
    virtual void stopLogger() = 0;
};

}
