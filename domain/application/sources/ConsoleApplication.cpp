#include "domain/framework/includes/LibModbusMaster.hpp"
#include "domain/gateway/includes/ModbusGateway.hpp"
#include "domain/gateway/includes/ModbusMasterController.hpp"
#include "domain/gateway/includes/ModbusSlaveController.hpp"
#include "domain/gateway/includes/ModbusSlaveFactory.hpp"

#include <iostream>

int main()
{
    using namespace Gateway;

    // external master
    const std::string ipAddrExtMaster = "127.0.0.1";
    const int portExtMaster = 5002; // no private port use (Modbus default := 502)
    const Entity::ModbusDataMapping mbDataMapping = {0, 0, 0, 0, 10, 10, 10, 10};

    // external slave
    const std::string ipAddrExtSlave = "127.0.0.1";
    const int portExtSlave = 5002;

    // get Modbus slave instance from factory and set up listening
    auto mbSlave = ModbusSlaveFactory::createModbusSlave(ModbusSlaveFramework::LIBMODBUS);

    // TODO(Markus2101, 24.05.2020): get Modbus master instance from factory
    std::shared_ptr<ModbusMaster> mbMaster = std::make_shared<Framework::LibModbusMaster>();

    // create Modbus master controller and connect to external Modbusslave
    auto mbMasterController = std::make_shared<ModbusMasterController>(mbMaster, ipAddrExtSlave, portExtSlave);
    mbMasterController->connect();

    // set up Modbus gateway
    auto mbGateway = std::make_shared<ModbusGateway>(mbMasterController);

    // create Modbus controller and start it up
    auto mbSlaveController =
      std::make_unique<ModbusSlaveController>(mbSlave, mbGateway, mbDataMapping, ipAddrExtMaster, portExtMaster);
    mbSlaveController->waitForIncomingConnection();
    mbSlaveController->run();
    mbSlaveController->closeConnection();
}
