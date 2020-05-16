#include "domain/gateway/includes/ModbusGateway.hpp"
#include "domain/gateway/includes/ModbusSlaveController.hpp"
#include "domain/gateway/includes/ModbusSlaveFactory.hpp"

#include <iostream>

int main()
{
    using namespace Gateway;

    const std::string ipAddr = "127.0.0.1";
    const int port = 5002; // no private port use (Modbus default := 502)
    const ModbusDataMapping mbDataMapping = {0, 0, 0, 0, 10, 10, 10, 10};

    // get Modbus slave instance from factory and set up listening
    auto mbSlave = ModbusSlaveFactory::createModbusSlave(ModbusSlaveFramework::LIBMODBUS);

    // set up Modbus gateway
    auto mbGateway = std::make_shared<ModbusGateway>();

    // create Modbus controller and start it up
    auto mbSlaveController = std::make_unique<ModbusSlaveController>(mbSlave, mbGateway, mbDataMapping, ipAddr, port);
    mbSlaveController->waitForIncomingConnection();
    mbSlaveController->run();
    mbSlaveController->closeConnection();
}
