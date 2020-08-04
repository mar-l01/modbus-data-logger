#pragma once

#include "domain/entity/includes/ModbusLoggerConfiguration.hpp"
#include "domain/logging/interfaces/FileLoggerController.hpp"

#include "spdlog/sinks/rotating_file_sink.h"
#include <memory>

namespace Framework {

class SpdlogFileController : public Logging::FileLoggerController
{
public:
    SpdlogFileController();

    void startLogger() override;
    void stopLogger() override;
    void setLogConfiguration(const Entity::ModbusLoggerConfiguration& mbLogConfig) override;
    void logModbusData(const std::shared_ptr<Entity::ModbusDataLog>& mbModbusData) override;

private:
    Entity::ModbusLoggerConfiguration m_mbLoggerConfig;
    std::shared_ptr<spdlog::logger> m_mbDataLogger;
};

}
