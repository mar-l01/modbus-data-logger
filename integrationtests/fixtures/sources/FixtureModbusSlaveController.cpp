#include "integrationtests/fixtures/includes/FixtureModbusSlaveController.hpp"

#include "domain/framework/includes/LibModbusSlave.hpp"
#include "integrationtests/fixtures/includes/TestConstants.hpp"

#include "gtest/gtest.h"


namespace Fixture {

using namespace Gateway;

FixtureModbusSlaveController::FixtureModbusSlaveController()
    : m_modbusSlave(nullptr)
    , m_socket(-1)
{}

void FixtureModbusSlaveController::setUp(const int nbIter)
{
    m_modbusSlave = std::make_unique<Framework::LibModbusSlave>();
    ASSERT_NE(m_modbusSlave, nullptr);
    m_modbusSlave->setModbusDataMapping(FixtureTestConstants::MODBUS_DATA_MAPPING);

    // let slave run on localhost:5002 accepting one connection
    m_modbusSlave->bind(FixtureTestConstants::MODBUS_IP_ADDRESS_INTERNAL_SLAVE,
                        FixtureTestConstants::MODBUS_PORT_INTERNAL_SLAVE);
    m_socket = m_modbusSlave->listen(1);
    ASSERT_NE(m_socket, -1);
    m_modbusSlave->accept(m_socket);

    // run infinite Modbus slave loop
    runRequestResponseLoop(nbIter);

    // connection is closed afterwards
    m_modbusSlave->close();
}

void FixtureModbusSlaveController::runRequestResponseLoop(const int nbIter)
{
    int reqLen = 0;
    auto modbusRequest = std::make_shared<Entity::ModbusTcpRequest>();

    // infinite request loop
    for (int i = 0; i < nbIter; ++i) {
        do {
            reqLen = m_modbusSlave->receive(modbusRequest);
        } while (reqLen == 0); // 0:= indication request ignored

        // error in receiving request
        if (reqLen == -1) {
            FAIL();
            break;
        }

        // TODO(Markus2101, 14.05.2020): currently reply() argument is not required
        auto modbusResponse = std::make_shared<Entity::ModbusTcpResponse>();
        reqLen = m_modbusSlave->reply(modbusResponse);
        if (reqLen == -1) {
            FAIL();
            break;
        }
    }
}

}
