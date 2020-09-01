#include "domain/application/includes/ModbusDataLoggerFacadeImpl.hpp"

#include "domain/gateway/interfaces/ModbusMasterController.hpp"
#include "domain/gateway/interfaces/ModbusSlaveController.hpp"
#include "domain/logging/interfaces/FileLogger.hpp"
#include "domain/utility/interfaces/Timer.hpp"

#include "spdlog/spdlog.h"

namespace Application {

ModbusDataLoggerFacadeImpl::ModbusDataLoggerFacadeImpl(
  const std::shared_ptr<Gateway::ModbusMasterController>& mbMasterController,
  const std::shared_ptr<Gateway::ModbusSlaveController>& mbSlaveController,
  const std::shared_ptr<Logging::FileLogger>& fileLogger, const std::shared_ptr<Utility::Timer>& timer)
    : m_mbMasterController(mbMasterController)
    , m_mbSlaveController(mbSlaveController)
    , m_fileLogger(fileLogger)
    , m_timer(timer)
{}

void ModbusDataLoggerFacadeImpl::startModbusCommunication()
{
    m_mbMasterController->connect();

    // run Modbus slave process loop in additional thread
    auto futureObj = m_threadStopSignal.get_future();
    m_mbSlaveThread = std::thread(&ModbusDataLoggerFacadeImpl::runModbusSlaveProcess, this, std::move(futureObj));

    // set action to be invoked once application timeout was received
    m_timer->callOnTimeout([this]() {
        m_threadStopSignal.set_value();
        SPDLOG_INFO("Timeout reached!");
        m_mbSlaveThread.join();
    });
}

void ModbusDataLoggerFacadeImpl::stopModbusCommunication()
{
    // trigger stop of Modbus process loop and wait for thread to be stopped
    m_threadStopSignal.set_value();
    m_mbSlaveThread.join();
    SPDLOG_INFO("Stopped Modbus communication!");

    // disconnect from communication partners
    m_mbSlaveController->disconnect();
    m_mbMasterController->disconnect();
}

void ModbusDataLoggerFacadeImpl::startLogger()
{
    m_fileLogger->startLogging();
}

void ModbusDataLoggerFacadeImpl::stopLogger()
{
    m_fileLogger->stopLogging();
}

void ModbusDataLoggerFacadeImpl::runModbusSlaveProcess(std::future<void> futureObj)
{
    // run Modbus slave until 'stopModbusCommunication()' was triggered
    do {
        m_mbSlaveController->waitForIncomingConnection();
        m_mbSlaveController->run();
    } while (futureObj.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout);
}

}
