#pragma once

#include "domain/entity/includes/ModbusLoggerConfiguration.hpp"
#include "domain/logging/interfaces/FileLoggerController.hpp"

#include "spdlog/sinks/rotating_file_sink.h"
#include <memory>

namespace Logging {

class SpdlogFileController : public FileLoggerController
{
public:
    SpdlogFileController();

    void startLogger() override;
    void stopLogger() override;
    void setLogConfiguration(const Entity::ModbusLoggerConfiguration& mbLogConfig) override;

private:
    Entity::ModbusLoggerConfiguration m_mbLoggerConfig;
    std::shared_ptr<spdlog::logger> m_mbDataLogger;
};

}