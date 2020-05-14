#include "domain/gateway/includes/ModbusSlaveFactory.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using namespace Gateway;

TEST(TestModbusSlaveFactory, createModbusSlaveReturnLibModbusSlave)
{
    auto libModbusSlave = ModbusSlaveFactory::createModbusSlave(ModbusSlaveFramework::LIBMODBUS);
    EXPECT_NE(libModbusSlave, nullptr);
}

TEST(TestModbusSlaveFactory, createModbusSlaveReturnNullptr)
{
    auto noModbusSlaveInstance = ModbusSlaveFactory::createModbusSlave(ModbusSlaveFramework::OTHER_MODBUS_FRAMEWORK);
    EXPECT_EQ(noModbusSlaveInstance, nullptr);
}

}
