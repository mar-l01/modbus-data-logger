#include "ui/facade/includes/ModbusDataLoggerFacadeUIWrapper.hpp"

#include "domain/application/interfaces/ModbusDataLoggerFacade.hpp"

namespace Facade {

ModbusDataLoggerFacadeUIWrapper::ModbusDataLoggerFacadeUIWrapper(
  std::shared_ptr<Application::ModbusDataLoggerFacade> mbDataLoggerFacade, QObject* parent)
    : QObject(parent)
    , m_mbDataLoggerFacade(std::move(mbDataLoggerFacade))
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
    emit applicationStateChanged(currentApplicationState);
}

void ModbusDataLoggerFacadeUIWrapper::onStartListenForApplicationStateChanges()
{
    m_applicationStateConnection =
      m_mbDataLoggerFacade->addApplicationStateListener([this](const Application::ApplicationState& applicationState) {
          emit applicationStateChanged(applicationState);
      });
}

void ModbusDataLoggerFacadeUIWrapper::onStopListenForApplicationStateChanges()
{
    m_applicationStateConnection.reset();
}

}
