#include "domain/gateway/includes/ModbusSlaveFactory.hpp"

int main()
{
    using namespace Gateway;

    // get Modbus slave instance from factory
    auto mbSlave = ModbusSlaveFactory::createModbusSlave(ModbusSlaveFramework::LIBMODBUS);
}
