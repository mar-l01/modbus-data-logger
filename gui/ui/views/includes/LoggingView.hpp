#pragma once

#include <QObject>
#include <memory>

namespace Application {
enum class ApplicationState : unsigned char;
}

namespace Facade {
class ModbusDataLoggerSignals;
}

namespace Views {

class LoggingView : public QObject
{
    Q_OBJECT

public:
    LoggingView(const std::shared_ptr<Facade::ModbusDataLoggerSignals>& mbDataLoggerSignals);

public slots:
    void onApplicationStateChanged(Application::ApplicationState applicationState);
    void onLoggingStarted();
    void onLoggingStopped();

private:
    std::shared_ptr<Facade::ModbusDataLoggerSignals> m_mbDataLoggerSignals;
};

}
