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
    , m_currentApplicationState(ApplicationState::STOPPED)
    , m_threadStopSignal(std::make_shared<std::promise<void>>())
{}

void ModbusDataLoggerFacadeImpl::startModbusCommunication()
{
    updateApplicationState(ApplicationState::STARTING);
    m_mbMasterController->connect();
    updateApplicationState(ApplicationState::STARTED);

    // run Modbus slave process loop in additional thread
    auto futureObj = m_threadStopSignal->get_future();
    m_mbSlaveThread = std::thread(&ModbusDataLoggerFacadeImpl::runModbusSlaveProcess, this, std::move(futureObj));

    // stop Modbus communication on application timeout
    m_timer->callOnTimeout([this]() {
        SPDLOG_INFO("Timeout reached!");
        closeConnectionToModbusComponents();
    });
}

void ModbusDataLoggerFacadeImpl::stopModbusCommunication()
{
    m_timer->stop();
    updateApplicationState(ApplicationState::STOPPING);
    closeConnectionToModbusComponents();
}

void ModbusDataLoggerFacadeImpl::startLogger()
{
    m_fileLogger->startLogging();
}

void ModbusDataLoggerFacadeImpl::stopLogger()
{
    m_fileLogger->stopLogging();
}

ApplicationState ModbusDataLoggerFacadeImpl::getCurrentApplicationState()
{
    return m_currentApplicationState;
}

std::shared_ptr<Framework::ScopedConnection> ModbusDataLoggerFacadeImpl::addApplicationStateListener(
  Framework::SignalCallback<ApplicationState> signalCallback)
{
    return std::make_shared<Framework::ScopedConnection>(m_currentApplicationStateEvent.connect(signalCallback));
}

void ModbusDataLoggerFacadeImpl::runModbusSlaveProcess(std::future<void> futureObj)
{
    // run Modbus slave until 'stopModbusCommunication()' was triggered
    do {
        m_mbSlaveController->waitForIncomingConnection();

        updateApplicationState(ApplicationState::RUNNING);
        m_mbSlaveController->run();
    } while (futureObj.wait_for(std::chrono::milliseconds(10)) == std::future_status::timeout);
}

void ModbusDataLoggerFacadeImpl::closeConnectionToModbusComponents()
{
    // trigger stop of Modbus process loop and wait for thread to be stopped
    try {
        m_threadStopSignal->set_value();
    } catch (const std::future_error& futureError) {
        SPDLOG_ERROR("Failed to set promise value: '{}'!", futureError.what());
    }
    m_mbSlaveThread.join();
    SPDLOG_INFO("Stopped Modbus communication!");

    // disconnect from communication partners
    m_mbSlaveController->disconnect();
    m_mbMasterController->disconnect();
    updateApplicationState(ApplicationState::STOPPED);
}

void ModbusDataLoggerFacadeImpl::updateApplicationState(const ApplicationState newApplicationState)
{
    // update and invoke signal callback only if state changes
    if (m_currentApplicationState != newApplicationState) {
        m_currentApplicationState = newApplicationState;
        m_currentApplicationStateEvent(m_currentApplicationState);
    }
}

}
