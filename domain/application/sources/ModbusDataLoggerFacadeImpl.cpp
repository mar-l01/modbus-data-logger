#include "domain/application/includes/ModbusDataLoggerFacadeImpl.hpp"

#include "domain/framework/includes/FileLoggerControllerFactory.hpp"
#include "domain/framework/includes/FileReaderFactory.hpp"
#include "domain/gateway/includes/ModbusComponentsFactory.hpp"
#include "domain/gateway/includes/ModbusGateway.hpp"
#include "domain/gateway/includes/ModbusMasterController.hpp"
#include "domain/gateway/includes/ModbusSlaveController.hpp"
#include "domain/logging/includes/ModbusDataLogger.hpp"
#include "domain/utility/includes/TimerFactory.hpp"
#include "domain/utility/interfaces/Timer.hpp"

#include "spdlog/spdlog.h"

namespace Application {

ModbusDataLoggerFacadeImpl::ModbusDataLoggerFacadeImpl(const std::string& mbConfigFilepath)
{
    createModbusGateway(mbConfigFilepath);
}

void ModbusDataLoggerFacadeImpl::startModbusCommunication()
{
    m_mbMasterController->connect();
}

void ModbusDataLoggerFacadeImpl::stopModbusCommunication()
{
    m_mbSlaveController->closeConnection();
    m_mbMasterController->closeConnection();
}

void ModbusDataLoggerFacadeImpl::startLogger()
{
    m_dataLogger->startLogging();
}

void ModbusDataLoggerFacadeImpl::stopLogger()
{
    m_dataLogger->stopLogging();
}

void ModbusDataLoggerFacadeImpl::createModbusGateway(const std::string& mbConfigFilepath)
{
    // read in Modbus configuration (file is provided via first console argument)
    auto fileReader = Framework::FileReaderFactory::createFileReader(Framework::FileReaderFramework::NLOHMANN_JSON);
    fileReader->readConfigurationFile(mbConfigFilepath);
    const auto& mbConfig = fileReader->getModbusConfiguration();

    // get Modbus slave instance from factory and set up listening
    auto mbSlave = Gateway::ModbusComponentsFactory::createModbusSlave(Gateway::ModbusComponentsFramework::LIBMODBUS);

    // get Modbus master instance from factory
    auto mbMaster = Gateway::ModbusComponentsFactory::createModbusMaster(Gateway::ModbusComponentsFramework::LIBMODBUS);

    // create Modbus master controller and connect to external Modbusslave
    m_mbMasterController =
      std::make_shared<Gateway::ModbusMasterController>(mbMaster, mbConfig.ipAddrExtSlave, mbConfig.portExtSlave);
    m_mbMasterController->setTimeout(mbConfig.modbusTimeout);

    // create data logger
    auto fileLoggerController =
      Framework::FileLoggerControllerFactory::createFileLoggerController(Framework::LoggingFramework::SPDLOG);
    m_dataLogger = std::make_shared<Logging::ModbusDataLogger>(fileLoggerController);

    // set up Modbus gateway
    auto mbGateway = std::make_shared<Gateway::ModbusGateway>(m_mbMasterController, m_dataLogger);

    // create timer instance with a loop frequency of 1 ms
    std::atomic_bool timeoutStop = false;
    std::shared_ptr<Utility::Timer> timerInstance = Utility::TimerFactory::createLoopTimer(1);
    timerInstance->callOnTimeout(mbConfig.applicationTimeout, [&timeoutStop]() {
        timeoutStop = true;
        SPDLOG_INFO("Timeout reached!");
    });

    // create Modbus controller and start it up
    auto m_mbSlaveController = std::make_unique<Gateway::ModbusSlaveController>(
      mbSlave, mbGateway, timerInstance, mbConfig.dataMapping, mbConfig.ipAddrIntSlave, mbConfig.portIntSlave);

    // run Modbus slave until 'stop' was received by timeout
    // (no reconnection will be triggered then)
    do {
        m_mbSlaveController->waitForIncomingConnection();
        m_mbSlaveController->run();
    } while (not timeoutStop);
}

}
