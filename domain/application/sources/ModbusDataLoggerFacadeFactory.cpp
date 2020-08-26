#include "domain/application/includes/ModbusDataLoggerFacadeFactory.hpp"

#include "domain/application/includes/ModbusDataLoggerFacadeImpl.hpp"
#include "domain/gateway/includes/ModbusGateway.hpp"
#include "domain/gateway/includes/ModbusMasterControllerImpl.hpp"
#include "domain/gateway/includes/ModbusSlaveControllerImpl.hpp"
#include "domain/logging/includes/ModbusDataLogger.hpp"
#include "domain/utility/includes/TimerFactory.hpp"
#include "domain/utility/interfaces/Timer.hpp"

#include "spdlog/spdlog.h"

namespace Application {

std::shared_ptr<ModbusDataLoggerFacade> ModbusDataLoggerFacadeFactory::createModbusDataLoggerFacade(
  const std::string& mbConfigFilepath, const FrameworkDependencies& frameworkDependencies)
{
    // read in Modbus configuration
    auto fileReader = Framework::FileReaderFactory::createFileReader(frameworkDependencies.fileReaderFramework);
    fileReader->readConfigurationFile(mbConfigFilepath);
    const auto& mbConfig = fileReader->getModbusConfiguration();

    // get Modbus master instance from factory
    auto mbMaster = Gateway::ModbusComponentsFactory::createModbusMaster(frameworkDependencies.modbusMasterFramework);

    // get Modbus slave instance from factory
    auto mbSlave = Gateway::ModbusComponentsFactory::createModbusSlave(frameworkDependencies.modbusSlaveFramework);

    // create Modbus master controller
    auto mbMasterController =
      std::make_shared<Gateway::ModbusMasterControllerImpl>(mbMaster, mbConfig.ipAddrExtSlave, mbConfig.portExtSlave);
    mbMasterController->setTimeout(mbConfig.modbusTimeout);

    // create data logger
    auto fileLoggerController =
      Framework::FileLoggerControllerFactory::createFileLoggerController(frameworkDependencies.loggingFramework);
    auto dataLogger = std::make_shared<Logging::ModbusDataLogger>(fileLoggerController);

    // set up Modbus gateway
    auto mbGateway = std::make_shared<Gateway::ModbusGateway>(mbMasterController, dataLogger);

    // create timer instance with a loop frequency of 1 ms
    std::atomic_bool timeoutStop = false;
    std::shared_ptr<Utility::Timer> timerInstance = Utility::TimerFactory::createLoopTimer(1);
    timerInstance->callOnTimeout(mbConfig.applicationTimeout, [&timeoutStop]() {
        timeoutStop = true;
        SPDLOG_INFO("Timeout reached!");
    });

    // create Modbus slave controller and start it up
    auto mbSlaveController = std::make_shared<Gateway::ModbusSlaveControllerImpl>(
      mbSlave, mbGateway, timerInstance, mbConfig.dataMapping, mbConfig.ipAddrIntSlave, mbConfig.portIntSlave);

    // create facade (cast required components to their interface type)
    std::shared_ptr<ModbusDataLoggerFacade> mbDataLoggerFacade = std::make_shared<ModbusDataLoggerFacadeImpl>(
      std::dynamic_pointer_cast<Gateway::ModbusMasterController>(mbMasterController),
      std::dynamic_pointer_cast<Gateway::ModbusSlaveController>(mbSlaveController), dataLogger);

    return mbDataLoggerFacade;
}

}
