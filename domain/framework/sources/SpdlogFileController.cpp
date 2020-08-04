#include "domain/framework/includes/SpdlogFileController.hpp"

#include "domain/entity/interfaces/ModbusDataLog.hpp"

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

void SpdlogFileController::logModbusData(const std::shared_ptr<Entity::ModbusDataLog>& mbModbusData)
{
    // log only if logger is active
    if (m_mbDataLogger != nullptr) {
        spdlog::get(m_mbLoggerConfig.loggerName)->info(mbModbusData->convertToLogString());
    } else {
        SPDLOG_DEBUG("Trying to log, but logger is not active");
    }
}

}
