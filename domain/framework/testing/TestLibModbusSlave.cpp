#include "domain/framework/includes/LibModbusSlave.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using namespace Framework;
using namespace Gateway;

TEST(TestLibModbusSlave, ctor)
{
    EXPECT_NO_THROW(LibModbusSlave tstObj);
}

}
