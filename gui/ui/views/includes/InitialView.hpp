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

class InitialView : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isMbAppRunning MEMBER m_isMbAppRunning NOTIFY mbAppRunningChanged)
    Q_PROPERTY(bool isButtonEnabled MEMBER m_isButtonEnabled NOTIFY buttonEnabledChanged)
    Q_PROPERTY(bool isStartButtonVisible MEMBER m_isStartButtonVisible NOTIFY startButtonVisibilityChanged)

public:
    InitialView(const std::shared_ptr<Facade::ModbusDataLoggerSignals>& mbDataLoggerSignals);

    Q_INVOKABLE void startModbusApplication();
    Q_INVOKABLE void stopModbusApplication();

signals:
    void mbAppRunningChanged();
    void buttonEnabledChanged();
    void startButtonVisibilityChanged();

public slots:
    void onApplicationStateChanged(Application::ApplicationState applicationState);

private:
    std::shared_ptr<Facade::ModbusDataLoggerSignals> m_mbDataLoggerSignals;
    bool m_isMbAppRunning;
    bool m_isButtonEnabled;
    bool m_isStartButtonVisible;

    void enableButton(bool enableIt);
    void setStartButtonVisibility(bool isVisible);
};

}
