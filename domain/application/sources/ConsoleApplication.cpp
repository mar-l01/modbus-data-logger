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
    using namespace Gateway;

    std::string jsonFilePath = "../../resources/mbdl_config.json";
    auto fileReader = Framework::FileReaderFactory::createFileReader(Framework::FileReaderFramework::NLOHMANN_JSON);
    fileReader->readConfigurationFile(jsonFilePath);

    return 1;

    // use CTRL + C to stop application
    signal(SIGINT, signalHandler);

    // external master
    const std::string ipAddrExtMaster = "127.0.0.1";
    const int portExtMaster = 5002; // no private port use (Modbus default := 502)
    const Entity::ModbusDataMapping mbDataMapping = {0, 0, 0, 0, 10, 10, 10, 10};

    // external slave
    const std::string ipAddrExtSlave = "127.0.0.1";
    const int portExtSlave = 5002;
    const uint16_t timeout = 200; // 200 ms timeout waiting for response of external slave

    // get Modbus slave instance from factory and set up listening
    auto mbSlave = ModbusComponentsFactory::createModbusSlave(ModbusComponentsFramework::LIBMODBUS);

    // get Modbus master instance from factory
    auto mbMaster = ModbusComponentsFactory::createModbusMaster(ModbusComponentsFramework::LIBMODBUS);

    // create Modbus master controller and connect to external Modbusslave
    auto mbMasterController = std::make_shared<ModbusMasterController>(mbMaster, ipAddrExtSlave, portExtSlave);
    mbMasterController->connect();
    mbMasterController->setTimeout(timeout);

    // set up Modbus gateway
    auto mbGateway = std::make_shared<ModbusGateway>(mbMasterController);

    // create timer instance
    int applicationTimeout = 5000; // 5 seconds timeout
    std::atomic_bool timeoutStop = false;
    std::shared_ptr<Utility::Timer> timerInstance = std::make_shared<Utility::TimerImpl>();
    timerInstance->callOnTimeout(applicationTimeout, [&timeoutStop]() {
        timeoutStop = true;
        std::cerr << "[ConsoleApplication] Timeout reached!\n";
    });

    // create Modbus controller and start it up
    auto mbSlaveController = std::make_unique<ModbusSlaveController>(mbSlave, mbGateway, timerInstance, mbDataMapping,
                                                                     ipAddrExtMaster, portExtMaster);

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
