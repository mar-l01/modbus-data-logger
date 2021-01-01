#pragma once

#include <QObject>
#include <memory>

namespace Application {
class ModbusDataLoggerFacade;
}

namespace Views {

class InitialView : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isMbAppRunning MEMBER m_isMbAppRunning NOTIFY mbAppRunningChanged)

public:
    InitialView(const std::shared_ptr<Application::ModbusDataLoggerFacade>& mbDataLoggerFacade,
                QObject* parent = nullptr);

    Q_INVOKABLE void startModbusApplication();
    Q_INVOKABLE void stopModbusApplication();

signals:
    void mbAppRunningChanged();

private:
    std::shared_ptr<Application::ModbusDataLoggerFacade> m_mbDataLoggerFacade;
    bool m_isMbAppRunning;
};

}
