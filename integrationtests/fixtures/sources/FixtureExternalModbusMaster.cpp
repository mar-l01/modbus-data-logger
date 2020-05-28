#include "integrationtests/fixtures/includes/FixtureExternalModbusMaster.hpp"

#include "integrationtests/fixtures/includes/TestConstants.hpp"

#include "gtest/gtest.h"

namespace Fixture {

FixtureExternalModbusMaster::FixtureExternalModbusMaster() {}

void FixtureExternalModbusMaster::setUp()
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

    // make sure connection is set up
    ASSERT_NE(ec, -1);
}

void FixtureExternalModbusMaster::checkWriteReadModbusRequest()
{
    std::vector<uint16_t> readHoldingRegisters(FixtureTestConstants::MODBUS_NUMBER_HOLDING_REGISTERS);

    // send Modbus request writing a single holding register...
    auto rc = modbus_write_register(m_modbusContext.get(), FixtureTestConstants::MODBUS_START_ADDRESS_HOLDING_REGISTERS,
                                    0x1234);
    EXPECT_EQ(rc, 1);

    // ...which is again read now
    rc = modbus_read_registers(m_modbusContext.get(), FixtureTestConstants::MODBUS_START_ADDRESS_HOLDING_REGISTERS, 1,
                               readHoldingRegisters.data());
    EXPECT_EQ(rc, 1);

    // test for equality
    EXPECT_EQ(readHoldingRegisters[FixtureTestConstants::MODBUS_START_ADDRESS_HOLDING_REGISTERS], 0x1234);
}

void FixtureExternalModbusMaster::tearDown()
{
    modbus_close(m_modbusContext.get());
}

}
