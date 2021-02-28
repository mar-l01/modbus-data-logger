#pragma once

#include <QObject>

namespace Facade {

/**
 * @brief This class holds all signals which can be used to call the respective methods of
 * the domain facade. Throwing one of them will trigger a call to the corresponding method
 * of the domain facade in a thread-safety manner.
 */
class ModbusDataLoggerManager : public QObject
{
    Q_OBJECT

public:
    ModbusDataLoggerManager() {}

signals:
    void startModbusCommunication();
    void stopModbusCommunication();
    void startLogger();
    void stopLogger();
    void getCurrentApplicationState();
    void startListenForApplicationStateChanges();
    void stopListenForApplicationStateChanges();
};

}
