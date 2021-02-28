#pragma once

#include <QObject>
#include <QThread>
#include <memory>

namespace Facade {

class ModbusDataLoggerFacadeUIWrapper;

class ModbusDataLoggerThreadController : public QObject
{
    Q_OBJECT

public:
    ModbusDataLoggerThreadController(std::unique_ptr<ModbusDataLoggerFacadeUIWrapper>&& mbDataLoggerFacadeUIWrapper);
    ~ModbusDataLoggerThreadController();

signals:
    void startModbusCommunication();
    void stopModbusCommunication();
    void startLogger();
    void stopLogger();
    void getCurrentApplicationState();
    void startListenForApplicationStateChanges();
    void stopListenForApplicationStateChanges();

private:
    std::unique_ptr<ModbusDataLoggerFacadeUIWrapper> m_mbDataLoggerFacadeUIWrapper;
    QThread m_workerThread;
};

}
