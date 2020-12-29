#pragma once

#include <QObject>

namespace Views {

class InitialView : public QObject
{
    Q_OBJECT

public:
    explicit InitialView(QObject* parent = nullptr);

    Q_INVOKABLE void startModbusApplication();
    Q_INVOKABLE void stopModbusApplication();
};

}
