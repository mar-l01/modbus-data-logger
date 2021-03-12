#pragma once

#include "domain/entity/includes/ModbusConfiguration.hpp"
#include "domain/framework/interfaces/ConfigurationFileAccessor.hpp"

#include <QObject>
#include <memory>

namespace Views {

class ConfigurationView : public QObject
{
    Q_OBJECT

public:
    ConfigurationView(const std::shared_ptr<Framework::ConfigurationFileAccessor>& configFileAccessor);

private:
    std::shared_ptr<Framework::ConfigurationFileAccessor> m_configFileAccessor;
    Entity::ModbusConfiguration m_mbConfiguration;
};

}
