#include "ui/facade/includes/ModbusDataLoggerFacadeUIWrapper.hpp"

#include "domain/application/interfaces/ModbusDataLoggerFacade.hpp"
#include "ui/facade/includes/ModbusDataLoggerSignals.hpp"

namespace Facade {

ModbusDataLoggerFacadeUIWrapper::ModbusDataLoggerFacadeUIWrapper(
  std::shared_ptr<Application::ModbusDataLoggerFacade>&& mbDataLoggerFacade,
  const std::shared_ptr<ModbusDataLoggerSignals>& mbDataLoggerSignals)
    : m_mbDataLoggerFacade(std::move(mbDataLoggerFacade))
    , m_mbDataLoggerSignals(mbDataLoggerSignals)
{}

void ModbusDataLoggerFacadeUIWrapper::onStartModbusCommunication()
{
    m_mbDataLoggerFacade->startModbusCommunication();
}

void ModbusDataLoggerFacadeUIWrapper::onStopModbusCommunication()
{
    m_mbDataLoggerFacade->stopModbusCommunication();
}

void ModbusDataLoggerFacadeUIWrapper::onStartLogger()
{
    m_mbDataLoggerFacade->startLogger();
}

void ModbusDataLoggerFacadeUIWrapper::onStopLogger()
{
    m_mbDataLoggerFacade->stopLogger();
}

void ModbusDataLoggerFacadeUIWrapper::onGetCurrentApplicationState()
{
    auto currentApplicationState = m_mbDataLoggerFacade->getCurrentApplicationState();
    emit m_mbDataLoggerSignals->applicationStateChanged(currentApplicationState);
}

void ModbusDataLoggerFacadeUIWrapper::onStartListenForApplicationStateChanges()
{
    m_applicationStateConnection =
      m_mbDataLoggerFacade->addApplicationStateListener([this](const Application::ApplicationState& applicationState) {
          emit m_mbDataLoggerSignals->applicationStateChanged(applicationState);
      });
}

void ModbusDataLoggerFacadeUIWrapper::onStopListenForApplicationStateChanges()
{
    m_applicationStateConnection.reset();
}

}
