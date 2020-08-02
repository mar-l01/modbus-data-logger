#include "domain/framework/includes/SpdlogFileController.hpp"

#include "gtest/gtest.h"

namespace {

using namespace Framework;
using namespace Logging;

TEST(TestSpdlogFileController, ctor)
{
    EXPECT_NO_THROW(SpdlogFileController tstObj);
}

}
