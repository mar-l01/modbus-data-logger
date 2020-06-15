#include "domain/framework/includes/FileReaderFactory.hpp"
#include "domain/gateway/includes/ModbusComponentsFactory.hpp"
#include "domain/gateway/includes/ModbusGateway.hpp"
#include "domain/gateway/includes/ModbusMasterController.hpp"
#include "domain/gateway/includes/ModbusSlaveController.hpp"
#include "domain/utility/includes/TimerImpl.hpp"
#include "domain/utility/interfaces/Timer.hpp"

#include <iostream>
#include <signal.h>

namespace ModbusReconnection {
bool startUpModbusSlaveAgain = true;
}

// handler triggered on CTRL + C
void signalHandler(sig_atomic_t)
{
    ModbusReconnection::startUpModbusSlaveAgain = false;
}

int main()
{
    // use CTRL + C to stop application
    signal(SIGINT, signalHandler);

    using namespace Gateway;

    // read in Modbus configuration
    // TODO(Markus2101, 15.06.2020): replace with absolute filepath
    std::string jsonFilePath = "../../resources/mbdl_config.json";
    auto fileReader = Framework::FileReaderFactory::createFileReader(Framework::FileReaderFramework::NLOHMANN_JSON);
    fileReader->readConfigurationFile(jsonFilePath);
    const auto& mbConfig = fileReader->getModbusConfiguration();

    // get Modbus slave instance from factory and set up listening
    auto mbSlave = ModbusComponentsFactory::createModbusSlave(ModbusComponentsFramework::LIBMODBUS);

    // get Modbus master instance from factory
    auto mbMaster = ModbusComponentsFactory::createModbusMaster(ModbusComponentsFramework::LIBMODBUS);

    // create Modbus master controller and connect to external Modbusslave
    auto mbMasterController =
      std::make_shared<ModbusMasterController>(mbMaster, mbConfig.ipAddrExtSlave, mbConfig.portExtSlave);
    mbMasterController->connect();
    mbMasterController->setTimeout(mbConfig.modbusTimeout);

    // set up Modbus gateway
    auto mbGateway = std::make_shared<ModbusGateway>(mbMasterController);

    // create timer instance
    std::atomic_bool timeoutStop = false;
    std::shared_ptr<Utility::Timer> timerInstance = std::make_shared<Utility::TimerImpl>();
    timerInstance->callOnTimeout(mbConfig.applicationTimeout, [&timeoutStop]() {
        timeoutStop = true;
        std::cerr << "[ConsoleApplication] Timeout reached!\n";
    });

    // create Modbus controller and start it up
    auto mbSlaveController = std::make_unique<ModbusSlaveController>(
      mbSlave, mbGateway, timerInstance, mbConfig.dataMapping, mbConfig.ipAddrIntSlave, mbConfig.portIntSlave);

    // run Modbus slave until 'stop' was received by signal interrupt or timeout
    // (no reconnection will be triggered then)
    do {
        mbSlaveController->waitForIncomingConnection();
        mbSlaveController->run();
    } while (ModbusReconnection::startUpModbusSlaveAgain && not timeoutStop);

    // close external connection in the end
    mbSlaveController->closeConnection();
    mbMasterController->closeConnection();
}
