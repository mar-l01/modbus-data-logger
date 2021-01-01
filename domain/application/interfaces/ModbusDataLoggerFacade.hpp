#pragma once

namespace Application {

/**
 * @brief Enum that holds different application states, which will be set in following order:
 * - start application -----------------> STARTING
 * - application is up -----------------> STARTED
 * - external Modbus device connected --> RUNNING
 * - stop application ------------------> STOPPING
 * - application is stopped ------------> STOPPED
 */
enum class APPLICATION_STATE : unsigned char
{
    STARTING = 0,
    STARTED,
    RUNNING,
    STOPPING,
    STOPPED
};

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
