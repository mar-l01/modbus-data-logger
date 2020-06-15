#include "domain/framework/includes/JsonFileReader.hpp"

#include "gtest/gtest.h"

namespace {

using namespace Framework;

TEST(TestJsonFileReader, ctor)
{
    EXPECT_NO_THROW(JsonFileReader jFileReader);
}

}
