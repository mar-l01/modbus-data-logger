#include "domain/application/includes/ModbusDataLoggerFacadeFactory.hpp"

#include "domain/application/includes/ModbusDataLoggerFacadeImpl.hpp"
#include "domain/framework/includes/FileLoggerControllerFactory.hpp"
#include "domain/framework/includes/FileReaderFactory.hpp"
#include "domain/gateway/includes/ModbusComponentsFactory.hpp"
#include "domain/gateway/includes/ModbusGateway.hpp"
#include "domain/gateway/includes/ModbusMasterControllerImpl.hpp"
#include "domain/gateway/includes/ModbusSlaveControllerImpl.hpp"
#include "domain/logging/includes/ModbusDataLogger.hpp"
#include "domain/utility/includes/TimerFactory.hpp"
#include "domain/utility/interfaces/Timer.hpp"

#include "spdlog/spdlog.h"
#include <string>

namespace Application {

std::shared_ptr<ModbusDataLoggerFacade> ModbusDataLoggerFacadeFactory::createModbusDataLoggerFacade(
  const std::string& mbConfigFilepath, const FrameworkDependencies& frameworkDependencies)
{
    // helper to make sure, generated objects are not nullptr
    auto assertNotNullptr = [](const auto& interfacePointer, int line) {
        if (interfacePointer == nullptr) {
            throw std::string("[Line: " + std::to_string(line) + "]: Object is nullptr: Failed to create object!");
        }
    };
    // read in Modbus configuration
    auto fileReader = Framework::FileReaderFactory::createFileReader(frameworkDependencies.fileReaderFramework);
    assertNotNullptr(fileReader, __LINE__);
    fileReader->readConfigurationFile(mbConfigFilepath);
    const auto& mbConfig = fileReader->getModbusConfiguration();

    // get Modbus master instance from factory
    auto mbMaster = Gateway::ModbusComponentsFactory::createModbusMaster(frameworkDependencies.modbusMasterFramework);
    assertNotNullptr(mbMaster, __LINE__);

    // get Modbus slave instance from factory
    auto mbSlave = Gateway::ModbusComponentsFactory::createModbusSlave(frameworkDependencies.modbusSlaveFramework);
    assertNotNullptr(mbSlave, __LINE__);

    // create Modbus master controller
    auto mbMasterController =
      std::make_shared<Gateway::ModbusMasterControllerImpl>(mbMaster, mbConfig.ipAddrExtSlave, mbConfig.portExtSlave);
    mbMasterController->setTimeout(mbConfig.modbusTimeout);

    // create data logger
    auto fileLoggerController =
      Framework::FileLoggerControllerFactory::createFileLoggerController(frameworkDependencies.loggingFramework);
    assertNotNullptr(fileLoggerController, __LINE__);
    auto dataLogger = std::make_shared<Logging::ModbusDataLogger>(fileLoggerController);
    assertNotNullptr(dataLogger, __LINE__);

    // set up Modbus gateway
    auto mbGateway = std::make_shared<Gateway::ModbusGateway>(mbMasterController, dataLogger);
    assertNotNullptr(mbGateway, __LINE__);

    // create timer instance with a loop frequency of 1 ms
    std::shared_ptr<Utility::Timer> timerInstance = Utility::TimerFactory::createLoopTimer(1);
    assertNotNullptr(timerInstance, __LINE__);
    timerInstance->setTimeoutInMs(mbConfig.applicationTimeout);

    // create Modbus slave controller and start it up
    auto mbSlaveController = std::make_shared<Gateway::ModbusSlaveControllerImpl>(
      mbSlave, mbGateway, timerInstance, mbConfig.dataMapping, mbConfig.ipAddrIntSlave, mbConfig.portIntSlave);
    assertNotNullptr(mbSlaveController, __LINE__);

    // create facade (cast required components to their interface type)
    std::shared_ptr<ModbusDataLoggerFacade> mbDataLoggerFacade = std::make_shared<ModbusDataLoggerFacadeImpl>(
      std::dynamic_pointer_cast<Gateway::ModbusMasterController>(mbMasterController),
      std::dynamic_pointer_cast<Gateway::ModbusSlaveController>(mbSlaveController), dataLogger, timerInstance);
    assertNotNullptr(mbDataLoggerFacade, __LINE__);

    return mbDataLoggerFacade;
}

}
