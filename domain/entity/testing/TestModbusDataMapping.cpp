#include "domain/entity/includes/ModbusDataMapping.hpp"

#include "gtest/gtest.h"

namespace {

using namespace Entity;

TEST(TestModbusDataMapping, mappingsAreEqual)
{
    ModbusDataMapping mappingA = {0, 1, 2, 3, 4, 5, 6, 7};
    ModbusDataMapping mappingB = {0, 1, 2, 3, 4, 5, 6, 7};

    EXPECT_TRUE(mappingA == mappingB);
}

TEST(TestModbusDataMapping, mappingsAreNotEqual)
{
    ModbusDataMapping mappingA = {0, 1, 2, 3, 4, 5, 6, 8};
    ModbusDataMapping mappingB = {0, 1, 2, 3, 4, 5, 6, 7};

    EXPECT_FALSE(mappingA == mappingB);
}

}
