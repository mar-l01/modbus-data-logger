#pragma once

#include "domain/framework/includes/SignalEvent.hpp"

#include <QObject>
#include <memory>

namespace Application {
class ModbusDataLoggerFacade;
enum class ApplicationState : unsigned char;
}

namespace Facade {

/**
 * @brief This class is used as wrapper for the facade-pattern implemented in the domain.
 * It provides differents signals and slots to interact with the domain facade.
 * This class will be provided to an additional worker-thread and the communication will
 * take place over signal/slot mechanism.
 */
class ModbusDataLoggerFacadeUIWrapper : public QObject
{
    Q_OBJECT

public:
    ModbusDataLoggerFacadeUIWrapper(std::shared_ptr<Application::ModbusDataLoggerFacade> mbDataLoggerFacade,
                                    QObject* parent = nullptr);

public slots:
    /**
     * @brief Start Modbus communication via the Modbus gateway
     */
    void onStartModbusCommunication();

    /**
     * @brief Stop Modbus communication via the Modbus gateway
     */
    void onStopModbusCommunication();

    /**
     * @brief Start logging the Modbus communication
     */
    void onStartLogger();

    /**
     * @brief Stop logging the Modbus communication
     */
    void onStopLogger();

    /**
     * @brief Get the current application state
     */
    void onEmitCurrentApplicationState();

    /**
     * @brief Get informed about changes in the application state
     */
    void onStartListenForApplicationStateChanges();

    /**
     * @brief Stop getting informed about changes in the application state
     */
    void onStopListenForApplicationStateChanges();

signals:
    void applicationStateChanged(Application::ApplicationState state);

private:
    std::shared_ptr<Application::ModbusDataLoggerFacade> m_mbDataLoggerFacade;
    std::shared_ptr<Framework::ScopedConnection> m_applicationStateConnection;
};

}
