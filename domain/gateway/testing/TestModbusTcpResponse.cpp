/*TEST_F(TestModbusTcpMessageFrame, getReadBitValues)
{
    auto testObj = createTestObject();

    // read 2 bytes of coil values (1 bit represents a single coil)
    testObj->dataBytes = {0x02, 0b10001100, 0b00011010};

    auto readCoilValues = testObj->getReadBitValues();
    std::vector<uint8_t> expectedReadCoilValues = {0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1};
    EXPECT_EQ(readCoilValues, expectedReadCoilValues);
}

TEST_F(TestModbusTcpMessageFrame, getReadRegisterValues)
{
    auto testObj = createTestObject();

    // read 8 bytes of register values which represent 4 register
    testObj->dataBytes = {0x08, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

    auto readRegisterValues = testObj->getReadRegisterValues();
    std::vector<uint16_t> expectedReadRegisterValues = {0x1122, 0x3344, 0x5566, 0x7788};
    EXPECT_EQ(readRegisterValues, expectedReadRegisterValues);
}
TEST_F(TestModbusTcpRequest, getNumberOfBytesOfReadValues)
{
    auto testObj = createTestObject();

    // set number of bytes of read values
    testObj->dataBytes = {0x04};

    auto bytesOfReadVals = testObj->getNumberOfBytesOfReadValues();
    EXPECT_EQ(bytesOfReadVals, 0x04);
}
*/
