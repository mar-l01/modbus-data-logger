#pragma once

#include <QObject>
#include <QThread>
#include <memory>

namespace Facade {

class ModbusDataLoggerFacadeUIWrapper;
class ModbusDataLoggerSignals;

class ModbusDataLoggerThreadController : public QObject
{
    Q_OBJECT

public:
    ModbusDataLoggerThreadController(std::unique_ptr<ModbusDataLoggerFacadeUIWrapper>&& mbDataLoggerFacadeUIWrapper,
                                     const std::shared_ptr<ModbusDataLoggerSignals> mbDataLoggerSignals);
    ~ModbusDataLoggerThreadController();

private:
    std::unique_ptr<ModbusDataLoggerFacadeUIWrapper> m_mbDataLoggerFacadeUIWrapper;
    std::shared_ptr<ModbusDataLoggerSignals> m_mbDataLoggerSignals;
    QThread m_workerThread;
};

}
