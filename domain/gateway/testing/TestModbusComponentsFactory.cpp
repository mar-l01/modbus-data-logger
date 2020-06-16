#include "domain/gateway/includes/ModbusComponentsFactory.hpp"

#include "gtest/gtest.h"

namespace {

using namespace Gateway;

TEST(TestModbusComponentsFactory, createModbusSlaveReturnLibModbusSlave)
{
    auto libModbusSlave = ModbusComponentsFactory::createModbusSlave(ModbusComponentsFramework::LIBMODBUS);
    EXPECT_NE(libModbusSlave, nullptr);
}

TEST(TestModbusComponentsFactory, createModbusSlaveReturnNullptr)
{
    auto noModbusSlaveInstance =
      ModbusComponentsFactory::createModbusSlave(ModbusComponentsFramework::OTHER_MODBUS_FRAMEWORK);
    EXPECT_EQ(noModbusSlaveInstance, nullptr);
}

TEST(TestModbusComponentsFactory, createModbusMasterReturnLibModbusMaster)
{
    auto libModbusMaster = ModbusComponentsFactory::createModbusMaster(ModbusComponentsFramework::LIBMODBUS);
    EXPECT_NE(libModbusMaster, nullptr);
}

TEST(TestModbusComponentsFactory, createModbusMasterReturnNullptr)
{
    auto noModbusMasterInstance =
      ModbusComponentsFactory::createModbusMaster(ModbusComponentsFramework::OTHER_MODBUS_FRAMEWORK);
    EXPECT_EQ(noModbusMasterInstance, nullptr);
}

}
