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
{}

void ModbusDataLoggerFacadeImpl::startModbusCommunication()
{
    // allow only if application is not running
    if (!isApplicationState(ApplicationState::STOPPED)) {
        SPDLOG_ERROR("Could not start application because ApplicationState is not 'STOPPED', but '{}'",
                     static_cast<int>(m_currentApplicationState));
        return;
    }

    updateApplicationState(ApplicationState::STARTING);
    m_mbMasterController->connect();
    updateApplicationState(ApplicationState::STARTED);

    // stop Modbus communication on application timeout
    m_timer->callOnTimeout([this]() {
        SPDLOG_INFO("Timeout reached!");
        closeConnectionToModbusComponents();
    });

    // run Modbus slave process loop in additional thread
    m_mbSlaveThread = std::thread(&ModbusDataLoggerFacadeImpl::runModbusSlaveProcess, this);
}

void ModbusDataLoggerFacadeImpl::stopModbusCommunication()
{
    // allow only if application is running
    if (!isApplicationState(ApplicationState::RUNNING)) {
        SPDLOG_ERROR("Could not stop application because ApplicationState is not 'RUNNING', but '{}'",
                     static_cast<int>(m_currentApplicationState));
        return;
    }

    m_timer->stop();
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

void ModbusDataLoggerFacadeImpl::runModbusSlaveProcess()
{
    updateApplicationState(ApplicationState::RUNNING);

    // run Modbus slave until 'stopModbusCommunication()' was triggered
    do {
        m_mbSlaveController->waitForIncomingConnection();

        // do not continue, if application is not in RUNNING anymore
        if (!isApplicationState(ApplicationState::RUNNING)) {
            SPDLOG_DEBUG("Stopping processing loop: ApplicationState is not 'RUNNING'");
            break;
        }

        m_mbSlaveController->run();

        // do not start a new loop, if application is to be stopped
    } while (isApplicationState(ApplicationState::RUNNING));
}

void ModbusDataLoggerFacadeImpl::closeConnectionToModbusComponents()
{
    // trigger stop of Modbus process loop and wait for thread to be stopped
    updateApplicationState(ApplicationState::STOPPING);
    m_mbSlaveThread.join();
    SPDLOG_INFO("Stopped Modbus communication!");

    // disconnect from communication partners
    m_mbSlaveController->disconnect();
    m_mbMasterController->disconnect();
    updateApplicationState(ApplicationState::STOPPED);
}

void ModbusDataLoggerFacadeImpl::updateApplicationState(const ApplicationState newApplicationState)
{
    // for thread-safe method-invocation
    const std::lock_guard<std::mutex> lock(m_mutex);

    // update and invoke signal callback only if state changes
    if (m_currentApplicationState != newApplicationState) {
        m_currentApplicationState = newApplicationState;
        m_currentApplicationStateEvent(m_currentApplicationState);
    }
}

bool ModbusDataLoggerFacadeImpl::isApplicationState(ApplicationState cmpApplicationState)
{
    // for thread-safe method-invocation
    const std::lock_guard<std::mutex> lock(m_mutex);

    bool applicationStatesEqual = false;
    if (m_currentApplicationState == cmpApplicationState) {
        applicationStatesEqual = true;
    }

    return applicationStatesEqual;
}

}
