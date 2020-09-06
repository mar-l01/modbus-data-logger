#include "integrationtests/fixtures/includes/FixtureExternalModbusMaster.hpp"
#include "integrationtests/fixtures/includes/FixtureExternalModbusSlave.hpp"
#include "integrationtests/fixtures/includes/FixtureModbusDataLoggerFacade.hpp"

#include "gtest/gtest.h"

#include <thread>

namespace {

using namespace testing;
using namespace Fixture;

class TestModbusDataLoggerFacade
    : public Test
    , public FixtureModbusDataLoggerFacade
{
public:
    TestModbusDataLoggerFacade()
    {
        setUp();
    }
};

TEST_F(TestModbusDataLoggerFacade, startAndStopCommunication)
{
    // ---------- start communication ----------
    // given: external Modbus slave running in extra thread
    FixtureExternalModbusSlave mbExtSlave;
    std::thread mbExtSlaveThread(&FixtureExternalModbusSlave::setUp, &mbExtSlave);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // when: start communication, wait some time to make sure internal slave is running
    getFixture()->startModbusCommunication();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // then: external Modbus master is able to connect, full communication possible
    FixtureExternalModbusMaster mbExtMaster;
    mbExtMaster.setUp();

    // ---------- stop communication ----------
    // given: external Modbus components connected, facade is running
    // then: stop communication (use additional thread to not block main thread)
    std::thread stopComThread(&Application::ModbusDataLoggerFacade::stopModbusCommunication, getFixture());

    // wait for above threads to join
    mbExtMaster.tearDown();
    mbExtSlaveThread.join();
    stopComThread.join();

    // then: facade is not running, not able to connect from external side
    bool expectConnectionFailure = true;
    mbExtMaster.setUp(expectConnectionFailure);
}

}
