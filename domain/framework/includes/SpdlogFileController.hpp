#pragma once

#include "domain/entity/includes/ModbusLoggerConfiguration.hpp"
#include "domain/logging/interfaces/FileLoggerController.hpp"

#include "spdlog/sinks/rotating_file_sink.h"
#include <memory>

namespace Framework {

/**
 * @brief This class represents a kind of controller for a file-logger, implemented with the 'spdlog'-framework.
 * It derives from @ref Logging::FileLoggerController, which internally controls the logging of messages. It provides
 * methods to
 * - start logging,
 * - stop logging,
 * - log actual data and
 * - configure it.
 *
 * For further information about the 'spdlog'-framwork see: https://github.com/gabime/spdlog
 */
class SpdlogFileController : public Logging::FileLoggerController
{
public:
    /**
     * @brief Construct a new SpdlogFileController object.
     */
    SpdlogFileController();

    /**
     * @see FileLoggerController::startLogger
     */
    void startLogger() override;

    /**
     * @see FileLoggerController::stopLogger
     */
    void stopLogger() override;

    /**
     * @see FileLoggerController::setLogConfiguration
     */
    void setLogConfiguration(const Entity::ModbusLoggerConfiguration& mbLogConfig) override;

    /**
     * @see FileLoggerController::logModbusData
     */
    void logModbusData(const std::shared_ptr<Entity::ModbusDataLog>& mbModbusData) override;

private:
    Entity::ModbusLoggerConfiguration m_mbLoggerConfig;
    std::shared_ptr<spdlog::logger> m_mbDataLogger;
};

}
