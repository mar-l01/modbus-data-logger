#pragma once

#include <QObject>
#include <QThread>
#include <memory>

namespace Facade {

class ModbusDataLoggerFacadeUIWrapper;
class ModbusDataLoggerManager;

class ModbusDataLoggerThreadController : public QObject
{
    Q_OBJECT

public:
    ModbusDataLoggerThreadController(std::unique_ptr<ModbusDataLoggerFacadeUIWrapper>&& mbDataLoggerFacadeUIWrapper,
                                     const std::shared_ptr<ModbusDataLoggerManager> mbDataLoggerManager);
    ~ModbusDataLoggerThreadController();

private:
    std::unique_ptr<ModbusDataLoggerFacadeUIWrapper> m_mbDataLoggerFacadeUIWrapper;
    std::shared_ptr<ModbusDataLoggerManager> m_mbDataLoggerManager;
    QThread m_workerThread;
};

}
