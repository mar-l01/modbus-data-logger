#include "domain/gateway/includes/ModbusSlaveFactory.hpp"

int main()
{
    using namespace Gateway;

    const std::string ipAddr = "127.0.0.1";
    const int port = 502;
    const ModbusDataMapping mbDataMapping = {0, 0, 0, 0, 10, 10, 10, 10};

    // get Modbus slave instance from factory and set up listening
    auto mbSlave = ModbusSlaveFactory::createModbusSlave(ModbusSlaveFramework::LIBMODBUS);
    mbSlave->setupModbusContext(ipAddr, port);
    mbSlave->setupModbusMapping(mbDataMapping);
    mbSlave->listenAndAcceptIncomingConnection();
    mbSlave->processIncomingRequests();
}
