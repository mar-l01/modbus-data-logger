#include "integrationtests/fixtures/includes/FixtureModbusMasterController.hpp"

#include "domain/framework/includes/LibModbusMaster.hpp"
#include "integrationtests/fixtures/includes/TestConstants.hpp"

#include "gtest/gtest.h"

namespace Fixture {

FixtureModbusMasterController::FixtureModbusMasterController() {}

void FixtureModbusMasterController::setUp()
{
    m_modbusMaster = std::make_unique<Framework::LibModbusMaster>();
    ASSERT_NE(m_modbusMaster, nullptr);

    // connect to external Modbus slave
    m_modbusMaster->connect(FixtureTestConstants::MODBUS_IP_ADDRESS_EXTERNAL_SLAVE,
                            FixtureTestConstants::MODBUS_PORT_EXTERNAL_SLAVE);
}

void FixtureModbusMasterController::checkReadHoldingRegisterValue()
{
    auto readReg =
      m_modbusMaster->readHoldingRegisterValues(FixtureTestConstants::MODBUS_START_ADDRESS_HOLDING_REGISTERS, 1);

    EXPECT_EQ(readReg.second[0], 0x1111);
}

void FixtureModbusMasterController::tearDown()
{
    m_modbusMaster->close();
}

}
