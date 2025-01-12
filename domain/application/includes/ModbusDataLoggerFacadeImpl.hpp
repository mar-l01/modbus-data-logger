#pragma once

#include "domain/application/interfaces/ModbusDataLoggerFacade.hpp"

#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace Gateway {
// forward declarations
class ModbusSlaveController;
class ModbusMasterController;
}

namespace Logging {
// forward declaration
class FileLogger;
}

namespace Utility {
// forwad declaration
class Timer;
}

namespace Application {

class ModbusDataLoggerFacadeImpl : public ModbusDataLoggerFacade
{
public:
    ModbusDataLoggerFacadeImpl(const std::shared_ptr<Gateway::ModbusMasterController>& mbMasterController,
                               const std::shared_ptr<Gateway::ModbusSlaveController>& mbSlaveController,
                               const std::shared_ptr<Logging::FileLogger>& fileLogger,
                               const std::shared_ptr<Utility::Timer>& timer);

    /**
     * @see ModbusDataLoggerFacade::startModbusCommunication
     */
    void startModbusCommunication() override;

    /**
     * @see ModbusDataLoggerFacade::stopModbusCommunication
     */
    void stopModbusCommunication() override;

    /**
     * @see ModbusDataLoggerFacade::startLogger
     */
    void startLogger() override;

    /**
     * @see ModbusDataLoggerFacade::stopLogger
     */
    void stopLogger() override;

    /**
     * @see ModbusDataLoggerFacade::getCurrentApplicationState
     */
    ApplicationState getCurrentApplicationState() override;

    /**
     * @see ModbusDataLoggerFacade::addApplicationStateListener
     */
    std::shared_ptr<Framework::ScopedConnection> addApplicationStateListener(
      Framework::SignalCallback<ApplicationState> signalCallback) override;

private:
    std::shared_ptr<Gateway::ModbusMasterController> m_mbMasterController;
    std::shared_ptr<Gateway::ModbusSlaveController> m_mbSlaveController;
    std::shared_ptr<Logging::FileLogger> m_fileLogger;
    std::shared_ptr<Utility::Timer> m_timer;
    ApplicationState m_currentApplicationState;
    Framework::SignalEvent<ApplicationState> m_currentApplicationStateEvent;

    std::mutex m_mutex;
    std::thread m_mbSlaveThread;
    void runModbusSlaveProcess();
    void closeConnectionToModbusComponents();

    void updateApplicationState(const ApplicationState newApplicationState);
    bool isApplicationState(ApplicationState cmpApplicationState);
};

}
