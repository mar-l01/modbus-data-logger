#include "integrationtests/fixtures/includes/FixtureExternalModbusMaster.hpp"

#include "domain/entity/includes/ModbusTcpConstants.hpp"
#include "integrationtests/fixtures/includes/TestConstants.hpp"

#include "gtest/gtest.h"

#include <thread>

namespace Fixture {

FixtureExternalModbusMaster::FixtureExternalModbusMaster() {}

void FixtureExternalModbusMaster::setUp(bool expectConnectionFailure)
{
    // clang-format off
    m_modbusContext = std::move(std::unique_ptr<modbus_t, std::function<void(modbus_t*)>>(
        modbus_new_tcp(FixtureTestConstants::MODBUS_IP_ADDRESS_INTERNAL_SLAVE,
            FixtureTestConstants::MODBUS_PORT_INTERNAL_SLAVE),
                [this](modbus_t* mbCtx) {
                    // use custom-deleter provided by libmodbus
                    modbus_free(mbCtx);
                }));
    // clang-format on

    // make sure context is not null
    ASSERT_NE(m_modbusContext, nullptr);

    // connect
    auto ec = modbus_connect(m_modbusContext.get());

    if (expectConnectionFailure) {
        // connection should not have been established (e.g. due to timeout)
        ASSERT_EQ(ec, -1);

        // in case the connection was established somehow, make sure to disconnect again
        if (ec == 0) {
            tearDown();
        }
    } else {
        // make sure connection is set up
        ASSERT_NE(ec, -1);

        // try to re-connect once after 100ms, if a connection couldn't have been established
        if (ec == -1) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            auto ec = modbus_connect(m_modbusContext.get());
            ASSERT_NE(ec, -1);
        }
    }
}

void FixtureExternalModbusMaster::tearDown()
{
    modbus_close(m_modbusContext.get());
}

void FixtureExternalModbusMaster::checkWriteReadRequestBits()
{
    std::vector<uint8_t> readBits(FixtureTestConstants::MODBUS_NUMBER_COILS);

    std::vector<uint8_t> bitsToWrite{0, 1, 0, 1, 1, 1, 0, 1, 1};
    int nbBits = static_cast<int>(bitsToWrite.size());

    // send Modbus request writing several coils...
    auto rc = modbus_write_bits(m_modbusContext.get(), FixtureTestConstants::MODBUS_START_ADDRESS_COILS, nbBits,
                                bitsToWrite.data());
    EXPECT_EQ(rc, nbBits);

    // ...which are read again now
    rc = modbus_read_bits(m_modbusContext.get(), FixtureTestConstants::MODBUS_START_ADDRESS_COILS, nbBits,
                          readBits.data());
    EXPECT_EQ(rc, nbBits);

    // test for equality
    for (int i = 0; i < nbBits; ++i) {
        EXPECT_EQ(readBits[FixtureTestConstants::MODBUS_START_ADDRESS_COILS + i], bitsToWrite[i]);
    }
}

void FixtureExternalModbusMaster::checkWriteReadRequestRegisters()
{
    std::vector<uint16_t> readHoldingRegisters(FixtureTestConstants::MODBUS_NUMBER_HOLDING_REGISTERS);

    std::vector<uint16_t> registersToWrite{0x1234, 0x5678, 0x9abc};
    int nbRegisters = static_cast<int>(registersToWrite.size());

    // send Modbus request writing several holding register...
    auto rc =
      modbus_write_registers(m_modbusContext.get(), FixtureTestConstants::MODBUS_START_ADDRESS_HOLDING_REGISTERS,
                             nbRegisters, registersToWrite.data());
    EXPECT_EQ(rc, nbRegisters);

    // ...which are read again now
    rc = modbus_read_registers(m_modbusContext.get(), FixtureTestConstants::MODBUS_START_ADDRESS_HOLDING_REGISTERS,
                               nbRegisters, readHoldingRegisters.data());
    EXPECT_EQ(rc, nbRegisters);

    // test for equality
    for (int i = 0; i < nbRegisters; ++i) {
        EXPECT_EQ(readHoldingRegisters[FixtureTestConstants::MODBUS_START_ADDRESS_HOLDING_REGISTERS + i],
                  registersToWrite[i]);
    }
}

void FixtureExternalModbusMaster::checkUnsupportedFunctionCode()
{
    std::vector<uint16_t> readHoldingRegisters(FixtureTestConstants::MODBUS_NUMBER_HOLDING_REGISTERS);

    std::vector<uint16_t> registersToWrite{0x1234, 0x5678};
    int nbRegisters = static_cast<int>(registersToWrite.size());

    // unsupported function code (0x17 := write & read holding registers)
    auto rc = modbus_write_and_read_registers(
      m_modbusContext.get(), FixtureTestConstants::MODBUS_START_ADDRESS_HOLDING_REGISTERS, nbRegisters,
      registersToWrite.data(), FixtureTestConstants::MODBUS_START_ADDRESS_HOLDING_REGISTERS, nbRegisters,
      readHoldingRegisters.data());

    EXPECT_EQ(rc, -1);
}

void FixtureExternalModbusMaster::checkResponseTimeoutReadHoldingRegisters()
{
    std::vector<uint16_t> readHoldingRegisters(FixtureTestConstants::MODBUS_NUMBER_HOLDING_REGISTERS);

    // try reading one holding register -> response timeout should occur
    auto rc = modbus_read_registers(m_modbusContext.get(), FixtureTestConstants::MODBUS_START_ADDRESS_HOLDING_REGISTERS,
                                    1, readHoldingRegisters.data());

    // in case of timeout, Modbus exception code gets set as errno
    EXPECT_EQ(rc, -1);
    EXPECT_EQ(errno, EMBXGTAR); // equals GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND
}

void FixtureExternalModbusMaster::checkLogging()
{
    std::vector<uint16_t> readHoldingRegisters(FixtureTestConstants::MODBUS_NUMBER_HOLDING_REGISTERS);

    std::vector<uint16_t> registersToWrite{0x1234, 0x5678, 0x9abc};
    int nbRegisters = static_cast<int>(registersToWrite.size());

    // prerequisities: set holding register values
    auto rc =
      modbus_write_registers(m_modbusContext.get(), FixtureTestConstants::MODBUS_START_ADDRESS_HOLDING_REGISTERS,
                             nbRegisters, registersToWrite.data());
    ASSERT_EQ(rc, nbRegisters);

    // capture iostream log now
    std::stringstream buffer;
    std::streambuf* sbuf = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());

    // read previously written values and...
    rc = modbus_read_registers(m_modbusContext.get(), FixtureTestConstants::MODBUS_START_ADDRESS_HOLDING_REGISTERS,
                               nbRegisters, readHoldingRegisters.data());
    ASSERT_EQ(rc, nbRegisters);

    std::string expectedOutputRequest(
      "--- Modbus Request ---> \n<0002><0000><0006><ff><03><00><00><00><03>\n\tTransaction Id: 2\n\tProtocol Id: 0 "
      "(TCP/IP Protocol)\n\tLength: 6\n\tUnit Id: 255\n\tFunction Code: READ_HOLDING_REGISTER_VALUES\n\tData "
      "Bytes:\n\t|--Start Address: 0\n\t|--Number of values to read: 3\n");

    std::string expectedOutputResponse(
      "<--- Modbus Response --- \n<0002><0000><0009><ff><03><06><12><34><56><78><9a><bc>\n\tTransaction Id: "
      "2\n\tProtocol Id: 0 (TCP/IP Protocol)\n\tLength: 9\n\tUnit Id: 255\n\tFunction Code: "
      "READ_HOLDING_REGISTER_VALUES\n\tData Bytes:\n\t|--Number of bytes to follow: 6\n\t|--Holding Register (1): "
      "4660\n\t|--Holding Register (2): 22136\n\t|--Holding Register (3): 39612\n");

    std::string expectedCombinedStrings = expectedOutputRequest + expectedOutputResponse;

    // ..check if its the expected one
    EXPECT_STRCASEEQ(expectedCombinedStrings.c_str(), buffer.str().c_str());

    // redirect cout again
    std::cout.rdbuf(sbuf);
}

}
