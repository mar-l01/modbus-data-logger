#include "ui/facade/includes/ModbusDataLoggerThreadController.hpp"

#include "ui/facade/includes/ModbusDataLoggerFacadeUIWrapper.hpp"
#include "ui/facade/includes/ModbusDataLoggerSignals.hpp"

namespace Facade {

ModbusDataLoggerThreadController::ModbusDataLoggerThreadController(
  std::unique_ptr<ModbusDataLoggerFacadeUIWrapper>&& mbDataLoggerFacadeUIWrapper,
  const std::shared_ptr<ModbusDataLoggerSignals> mbDataLoggerSignals)
    : m_mbDataLoggerFacadeUIWrapper(std::move(mbDataLoggerFacadeUIWrapper))
    , m_mbDataLoggerSignals(mbDataLoggerSignals)
{
    m_mbDataLoggerFacadeUIWrapper->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, m_mbDataLoggerFacadeUIWrapper.get(), &QObject::deleteLater);

    // connect all signals to the respective slots representing the domain facade wrapper
    connect(mbDataLoggerSignals.get(), &ModbusDataLoggerSignals::startModbusCommunication,
            m_mbDataLoggerFacadeUIWrapper.get(), &ModbusDataLoggerFacadeUIWrapper::onStartModbusCommunication);
    connect(mbDataLoggerSignals.get(), &ModbusDataLoggerSignals::stopModbusCommunication,
            m_mbDataLoggerFacadeUIWrapper.get(), &ModbusDataLoggerFacadeUIWrapper::onStopModbusCommunication);
    connect(mbDataLoggerSignals.get(), &ModbusDataLoggerSignals::startLogger, m_mbDataLoggerFacadeUIWrapper.get(),
            &ModbusDataLoggerFacadeUIWrapper::onStartLogger);
    connect(mbDataLoggerSignals.get(), &ModbusDataLoggerSignals::stopLogger, m_mbDataLoggerFacadeUIWrapper.get(),
            &ModbusDataLoggerFacadeUIWrapper::onStopLogger);
    connect(mbDataLoggerSignals.get(), &ModbusDataLoggerSignals::getCurrentApplicationState,
            m_mbDataLoggerFacadeUIWrapper.get(), &ModbusDataLoggerFacadeUIWrapper::onGetCurrentApplicationState);
    connect(mbDataLoggerSignals.get(), &ModbusDataLoggerSignals::startListenForApplicationStateChanges,
            m_mbDataLoggerFacadeUIWrapper.get(),
            &ModbusDataLoggerFacadeUIWrapper::onStartListenForApplicationStateChanges);
    connect(mbDataLoggerSignals.get(), &ModbusDataLoggerSignals::stopListenForApplicationStateChanges,
            m_mbDataLoggerFacadeUIWrapper.get(),
            &ModbusDataLoggerFacadeUIWrapper::onStopListenForApplicationStateChanges);

    m_workerThread.start();
}

ModbusDataLoggerThreadController::~ModbusDataLoggerThreadController()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

}
