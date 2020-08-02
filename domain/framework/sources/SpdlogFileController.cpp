#include "domain/framework/includes/SpdlogFileController.hpp"

#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

namespace Framework {

SpdlogFileController::SpdlogFileController() {}

void SpdlogFileController::startLogger()
{
    try {
        // create rotating file logger using this factory method
        m_mbDataLogger =
          spdlog::rotating_logger_mt(m_mbLoggerConfig.loggerName, m_mbLoggerConfig.logFilePath,
                                     m_mbLoggerConfig.maxLogFileSizeInMb, m_mbLoggerConfig.maxNumberOfFiles);
    } catch (const spdlog::spdlog_ex& ex) {
        SPDLOG_ERROR("Failed to start logger: ", ex.what());
    }
}

void SpdlogFileController::stopLogger()
{
    // reset shared pointer
    m_mbDataLogger.reset();

    // remove logger from registry
    spdlog::drop(m_mbLoggerConfig.loggerName);
}

void SpdlogFileController::setLogConfiguration(const Entity::ModbusLoggerConfiguration& mbLogConfig)
{
    m_mbLoggerConfig = mbLogConfig;
}

void SpdlogFileController::logModbusData(
  const std::variant<Entity::ModbusTcpRequest, Entity::ModbusTcpResponse>& mbModbusData)
{
    // log only if logger is active
    if (m_mbDataLogger != nullptr) {
        // decide between ModbusTcpRequest or ModbusTcpResponse
        // TODO(Markus2101, 2020-08-02): Make a customm type which can be logged depending on
        // whether a ModbusRequest or a ModbusResponse is present. Idea: Base class which provides
        // a toString() method and both classes derive from it and hence the data can be logged easily
        std::string logMsg("");
        if (auto loggedData = std::get_if<Entity::ModbusTcpRequest>(&mbModbusData)) {
            logMsg = "ModbusTcpRequest";
        } else if (auto loggedData = std::get_if<Entity::ModbusTcpResponse>(&mbModbusData)) {
            logMsg = "ModbusTcpResponse";
        }

        spdlog::get(m_mbLoggerConfig.loggerName)->info("Logging Modbus data: ", logMsg);
    } else {
        SPDLOG_DEBUG("Trying to log, but logger is not active");
    }
}

}
