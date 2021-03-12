#include "ui/views/includes/ConfigurationView.hpp"

namespace Views {

ConfigurationView::ConfigurationView(const std::shared_ptr<Framework::ConfigurationFileAccessor>& configFileAccessor)
    : m_configFileAccessor(std::move(configFileAccessor))
    , m_mbConfiguration(m_configFileAccessor->getModbusConfiguration())
{}

}
