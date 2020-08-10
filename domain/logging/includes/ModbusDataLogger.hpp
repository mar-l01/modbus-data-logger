#pragma once

#include "domain/logging/interfaces/DataLogger.hpp"
#include "domain/logging/interfaces/FileLogger.hpp"
#include "domain/logging/interfaces/FileLoggerController.hpp"
#include "domain/logging/interfaces/RealTimeLogger.hpp"

#include "spdlog/sinks/rotating_file_sink.h"
#include <boost/signals2/signal.hpp>

namespace Entity {
// forward declarations
class ModbusTcpRequest;
class ModbusTcpResponse;
class ModbusLoggerConfiguration;
}

namespace Logging {

template<typename T>
using SignalEvent = boost::signals2::signal<void(const T&)>;

/**
 * @brief This class implements several interfaces, which are used for logging a Modbus communication:
 * - DataLogger: used to log either a Modbus request or a Modbus response
 * - RealTimeLogger: used for real-time notifications to subscribed clients
 * - FileLogger: used to log a Modbus communication to files
 */
class ModbusDataLogger
    : public DataLogger
    , public RealTimeLogger
    , public FileLogger
{
public:
    /**
     * @brief Construct a new ModbusDataLogger object.
     *
     * @param fileLoggerController An instance of @ref FileLoggerController which is responsible for logging the Modbus
     * communication to a file.
     */
    ModbusDataLogger(const std::shared_ptr<FileLoggerController>& fileLoggerController);

    // DataLogger interface
    /**
     * @see DataLogger::logModbusRequest
     */
    void logModbusRequest(const std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest) override;

    /**
     * @see DataLogger::logModbusResponse
     */
    void logModbusResponse(const std::shared_ptr<Entity::ModbusTcpResponse>& mbResponse) override;

    // RealTimeLogger interface
    /**
     * @see RealTimeLogger::addModbusRequestListener
     */
    std::shared_ptr<ScopedConnection> addModbusRequestListener(
      SignalCallback<Entity::ModbusTcpRequest> signalCallback) override;

    /**
     * @see RealTimeLogger::addModbusResponseListener
     */
    std::shared_ptr<ScopedConnection> addModbusResponseListener(
      SignalCallback<Entity::ModbusTcpResponse> signalCallback) override;

    // FileLogger interface
    /**
     * @see FileLogger::startLogging
     */
    void startLogging() override;

    /**
     * @see FileLogger::stopLogging
     */
    void stopLogging() override;

    /**
     * @see FileLogger::changeLogFileConfiguration
     */
    void changeLogFileConfiguration(const Entity::ModbusLoggerConfiguration& mbLogConfig) override;

private:
    SignalEvent<Entity::ModbusTcpRequest> m_mbRequestEvent;
    SignalEvent<Entity::ModbusTcpResponse> m_mbResponseEvent;
    const std::shared_ptr<FileLoggerController> m_fileLoggerController;
};

}
