#pragma once

#include "domain/logging/interfaces/DataLogger.hpp"
#include "domain/logging/interfaces/FileLogger.hpp"
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

class ModbusDataLogger
    : public DataLogger
    , public RealTimeLogger
    , public FileLogger
{
public:
    ModbusDataLogger();

    // DataLogger interface
    void logModbusRequest(const Entity::ModbusTcpRequest& mbRequest) override;
    void logModbusResponse(const Entity::ModbusTcpResponse& mbResponse) override;

    // RealTimerLogger interface
    std::shared_ptr<ScopedConnection> addModbusRequestListener(
      SignalCallback<Entity::ModbusTcpRequest> signalCallback) override;
    std::shared_ptr<ScopedConnection> addModbusResponseListener(
      SignalCallback<Entity::ModbusTcpResponse> signalCallback) override;

    // FileLogger interface
    void startLogging() override;
    void stopLogging() override;
    void changeLogFileConfiguration(const Entity::ModbusLoggerConfiguration& mbLogConfig) override;

private:
    SignalEvent<Entity::ModbusTcpRequest> m_mbRequestEvent;
    SignalEvent<Entity::ModbusTcpResponse> m_mbResponseEvent;
};

}
