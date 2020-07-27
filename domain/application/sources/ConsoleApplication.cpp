#include "domain/framework/includes/FileReaderFactory.hpp"
#include "domain/gateway/includes/ModbusComponentsFactory.hpp"
#include "domain/gateway/includes/ModbusGateway.hpp"
#include "domain/gateway/includes/ModbusMasterController.hpp"
#include "domain/gateway/includes/ModbusSlaveController.hpp"
#include "domain/utility/includes/TimerImpl.hpp"
#include "domain/utility/interfaces/Timer.hpp"

#include "spdlog/spdlog.h"
#include <signal.h>

namespace ModbusReconnection {
bool startUpModbusSlaveAgain = true;
}

// handler triggered on CTRL + C
void signalHandler(sig_atomic_t)
{
    ModbusReconnection::startUpModbusSlaveAgain = false;
}

int main(int argc, char* argv[])
{
    if (argc <= 1) {
        SPDLOG_ERROR("File path required!");
        SPDLOG_ERROR("Usage: ./modbus_data_logger <config-file-path>");
        return 1;
    }

    // use CTRL + C to stop application
    signal(SIGINT, signalHandler);

    // read in Modbus configuration (file is provided via first console argument)
    std::string jsonFilePath(argv[1]);
    auto fileReader = Framework::FileReaderFactory::createFileReader(Framework::FileReaderFramework::NLOHMANN_JSON);
    fileReader->readConfigurationFile(jsonFilePath);
    const auto& mbConfig = fileReader->getModbusConfiguration();

    // get Modbus slave instance from factory and set up listening
    auto mbSlave = Gateway::ModbusComponentsFactory::createModbusSlave(Gateway::ModbusComponentsFramework::LIBMODBUS);

    // get Modbus master instance from factory
    auto mbMaster = Gateway::ModbusComponentsFactory::createModbusMaster(Gateway::ModbusComponentsFramework::LIBMODBUS);

    // create Modbus master controller and connect to external Modbusslave
    auto mbMasterController =
      std::make_shared<Gateway::ModbusMasterController>(mbMaster, mbConfig.ipAddrExtSlave, mbConfig.portExtSlave);
    mbMasterController->connect();
    mbMasterController->setTimeout(mbConfig.modbusTimeout);

    // set up Modbus gateway
    auto mbGateway = std::make_shared<Gateway::ModbusGateway>(mbMasterController);

    // create timer instance
    std::atomic_bool timeoutStop = false;
    std::shared_ptr<Utility::Timer> timerInstance = std::make_shared<Utility::TimerImpl>();
    timerInstance->callOnTimeout(mbConfig.applicationTimeout, [&timeoutStop]() {
        timeoutStop = true;
        SPDLOG_INFO("Timeout reached!");
    });

    // create Modbus controller and start it up
    auto mbSlaveController = std::make_unique<Gateway::ModbusSlaveController>(
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
