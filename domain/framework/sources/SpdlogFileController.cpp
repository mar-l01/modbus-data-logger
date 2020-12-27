#include "domain/framework/includes/SpdlogFileController.hpp"

#include "domain/entity/interfaces/ModbusDataLog.hpp"

#include <filesystem>

namespace Framework {

SpdlogFileController::SpdlogFileController() {}

void SpdlogFileController::startLogger()
{
    // check if directory of given logger file exists (removing file name results in directory to check)
    const auto loggerDirectory = std::filesystem::path(m_mbLoggerConfig.logFilePath).remove_filename();
    if (!std::filesystem::exists(loggerDirectory)) {
        std::filesystem::create_directories(loggerDirectory);
    }

    try {
        // create rotating file logger using this factory method
        m_mbDataLogger =
          spdlog::rotating_logger_mt(m_mbLoggerConfig.loggerName, m_mbLoggerConfig.logFilePath,
                                     m_mbLoggerConfig.maxLogFileSizeInMb, m_mbLoggerConfig.maxNumberOfFiles);
    } catch (const spdlog::spdlog_ex& ex) {
        SPDLOG_ERROR("Failed to start logger: {}", ex.what());
    } catch (const std::exception& ex) {
        SPDLOG_ERROR("Failed to start logger: {}", ex.what());
    }
}

void SpdlogFileController::stopLogger()
{
    // remove logger from registry
    spdlog::drop(m_mbLoggerConfig.loggerName);

    // reset shared pointer
    m_mbDataLogger.reset();
}

void SpdlogFileController::setLogConfiguration(const Entity::ModbusLoggerConfiguration& mbLogConfig)
{
    m_mbLoggerConfig = mbLogConfig;
}

void SpdlogFileController::logModbusData(const std::shared_ptr<Entity::ModbusDataLog>& mbModbusData)
{
    try {
        // log only if logger is active
        if (m_mbDataLogger != nullptr) {
            spdlog::get(m_mbLoggerConfig.loggerName)->info(mbModbusData->convertToLogString());
        } else {
            SPDLOG_DEBUG("Trying to log, but logger is not active");
        }
    } catch (const std::exception& ex) {
        SPDLOG_ERROR("Failed to log message: {}", ex.what());
    }
}

}
