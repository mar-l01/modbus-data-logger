#include "domain/gateway/includes/ModbusSlaveFactory.hpp"
#include "domain/gateway/includes/ModbusSlaveHandler.hpp"

#include <iostream>

int main()
{
    using namespace Gateway;

    const std::string ipAddr = "127.0.0.1";
    const int port = 5002;
    const ModbusDataMapping mbDataMapping = {0, 0, 0, 0, 10, 10, 10, 10};

    // get Modbus slave instance from factory and set up listening
    auto mbSlave = ModbusSlaveFactory::createModbusSlave(ModbusSlaveFramework::LIBMODBUS);

    std::unique_ptr<ModbusSlaveHandler> mbSlaveHandler =
      std::make_unique<ModbusSlaveHandler>(std::move(mbSlave), mbDataMapping, ipAddr, port);

    mbSlaveHandler->connect();
    mbSlaveHandler->run();
    mbSlaveHandler->closeConnection();
}
