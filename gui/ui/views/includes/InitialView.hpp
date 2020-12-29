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

public:
    explicit InitialView(const std::shared_ptr<Application::ModbusDataLoggerFacade>& mbDataLoggerFacade,
                         QObject* parent = nullptr);

    Q_INVOKABLE void startModbusApplication();
    Q_INVOKABLE void stopModbusApplication();

private:
    std::shared_ptr<Application::ModbusDataLoggerFacade> m_mbDataLoggerFacade;
};

}