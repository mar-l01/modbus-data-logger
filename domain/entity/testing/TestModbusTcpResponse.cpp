#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include "gtest/gtest.h"

namespace {

using namespace Entity;

class TestModbusTcpResponse : public ::testing::Test
{
protected:
    std::unique_ptr<ModbusTcpResponse> createTestObject()
    {
        auto testObj = std::make_unique<ModbusTcpResponse>();

        return std::move(testObj);
    }
};

TEST_F(TestModbusTcpResponse, getReadBitValues)
{
    auto testObj = createTestObject();

    // 1 bit represents a single coil value
    std::vector<uint8_t> expectedReadCoilValues = {1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0};
    testObj->setReadValues(expectedReadCoilValues);

    auto readCoilValues = testObj->getReadBitValues();
    EXPECT_EQ(readCoilValues, expectedReadCoilValues);
}

TEST_F(TestModbusTcpResponse, getReadRegisterValues)
{
    auto testObj = createTestObject();

    // read 8 bytes of register values which represent 4 register
    std::vector<uint16_t> expectedReadRegisterValues = {0x1122, 0x3344, 0x5566, 0x7788};
    testObj->setReadValues(expectedReadRegisterValues);

    auto readRegisterValues = testObj->getReadRegisterValues();
    EXPECT_EQ(readRegisterValues, expectedReadRegisterValues);
}

TEST_F(TestModbusTcpResponse, getReadBitValuesEmpty)
{
    auto testObj = createTestObject();

    auto expectedEmptyBitVector = std::vector<uint8_t>();

    auto readBitValues = testObj->getReadBitValues();
    EXPECT_EQ(readBitValues, expectedEmptyBitVector);
}

TEST_F(TestModbusTcpResponse, getReadRegisterValuesEmpty)
{
    auto testObj = createTestObject();

    auto expectedEmptyRegisterVector = std::vector<uint16_t>();

    auto readRegisterValues = testObj->getReadRegisterValues();
    EXPECT_EQ(readRegisterValues, expectedEmptyRegisterVector);
}

}
