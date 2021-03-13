#include "domain/framework/includes/LibModbusMaster.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using namespace Framework;
using namespace Gateway;

TEST(TestLibModbusMaster, ctor)
{
    EXPECT_NO_THROW(LibModbusMaster tstObj);
}

TEST(TestLibModbusSlave, isConnectedReturnsFalseByDefault)
{
    LibModbusMaster tstObj;
    EXPECT_FALSE(tstObj.isConnected());
}

}
