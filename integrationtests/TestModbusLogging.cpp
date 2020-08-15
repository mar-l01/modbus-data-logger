#include "integrationtests/fixtures/includes/FixtureExternalModbusMaster.hpp"
#include "integrationtests/fixtures/includes/FixtureExternalModbusSlave.hpp"
#include "integrationtests/fixtures/includes/FixtureModbusGateway.hpp"
#include "integrationtests/fixtures/includes/TestConstants.hpp"

#include "gtest/gtest.h"

#include <chrono>
#include <thread>

namespace {

using namespace Fixture;

TEST(TestModbusLogging, checkLog)
{
    // create external test fixtures
    FixtureExternalModbusMaster mbExtMaster;
    FixtureExternalModbusSlave mbExtSlave;

    // run external Modbus slave in extra thread
    std::thread mbExtSlaveThread(&FixtureExternalModbusSlave::setUp, &mbExtSlave);

    // wait some ms to make sure external slave is up and running
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // create Modbus gateway fixture
    bool logOutputToConsole = true;
    FixtureModbusGateway mbGateway(logOutputToConsole);

    // run gateway in extra thread -> no reconnection needed
    std::thread mbGatewayThread(&FixtureModbusGateway::setUp, &mbGateway, 0);

    // wait some ms to make sure internal slave is up and running
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // connect to internal Modbus slave
    mbExtMaster.setUp();

    // check read request log
    mbExtMaster.checkLogging();

    // wait some ms to make sure internal slave is up and running
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // close connection
    mbExtMaster.tearDown();

    // join threads
    mbGatewayThread.join();
    mbExtSlaveThread.join();
}

}
