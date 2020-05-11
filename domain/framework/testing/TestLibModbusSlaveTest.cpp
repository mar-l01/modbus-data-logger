#include "domain/framework/includes/LibModbusSlave.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using namespace Framework;
using namespace Gateway;

TEST(TestLibModbusSlaveTest, initReturnTrue)
{
    LibModbusSlave tstObj;

    // init args
    std::string ipAddr = "127.0.0.1";
    int port = 5002;
    ModbusDataMapping mbDataMapping = {0, 1, 2, 3, 4, 5, 6, 7};

    // expect init method to return true
    auto isSuccessful = tstObj.init(ipAddr, port, mbDataMapping);
    EXPECT_EQ(isSuccessful, true);
}

TEST(TestLibModbusSlaveTest, initReturnFalse)
{
    LibModbusSlave tstObj;

    // init args
    std::string ipAddr = ""; // results in error
    int port = 5002;
    ModbusDataMapping mbDataMapping = {0, 1, 2, 3, 4, 5, 6, 7};

    // expect init method to return false
    auto isSuccessful = tstObj.init(ipAddr, port, mbDataMapping);
    EXPECT_EQ(isSuccessful, false);
}

}
