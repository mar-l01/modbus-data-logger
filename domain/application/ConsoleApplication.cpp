#include "domain/gateway/includes/ModbusSlaveFactory.hpp"

#include <iostream>

int main()
{
    using namespace Gateway;

    const std::string ipAddr = "127.0.0.1";
    const int port = 5002;
    const ModbusDataMapping mbDataMapping = {0, 0, 0, 0, 10, 10, 10, 10};

    // get Modbus slave instance from factory and set up listening
    auto mbSlave = ModbusSlaveFactory::createModbusSlave(ModbusSlaveFramework::LIBMODBUS);
    auto initSuccess = mbSlave->init(ipAddr, port, mbDataMapping);
    if (initSuccess) {
        mbSlave->run();
    } else {
        std::cout << "[ConsoleApplication] Failed to initialize Modbus slave with following parameters:\n";
        std::cout << "[ConsoleApplciation] - IP: " << ipAddr << '\n';
        std::cout << "[ConsoleApplciation] - Port: " << port << '\n';
        std::cout << "[ConsoleApplciation] - Data-Mapping: " << mbDataMapping << '\n';
        exit(1);
    }

    exit(0);
}
