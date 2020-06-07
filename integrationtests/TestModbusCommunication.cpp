#include "integrationtests/fixtures/includes/FixtureExternalModbusMaster.hpp"
#include "integrationtests/fixtures/includes/FixtureExternalModbusSlave.hpp"
#include "integrationtests/fixtures/includes/FixtureModbusGateway.hpp"

#include "gtest/gtest.h"

#include <chrono>
#include <thread>

namespace {

using namespace Fixture;

TEST(TestModbusCommunication, checkWriteReadOperation)
{
    // create external test fixtures
    FixtureExternalModbusMaster mbExtMaster;
    FixtureExternalModbusSlave mbExtSlave;

    // run external Modbus slave in extra thread
    std::thread mbExtSlaveThread(&FixtureExternalModbusSlave::setUp, &mbExtSlave);

    // wait some ms to make sure external slave is up and running
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // create Modbus gateway fixture
    FixtureModbusGateway mbGateway;

    // run gateway in extra thread -> no reconnection needed
    std::thread mbGatewayThread(&FixtureModbusGateway::setUp, &mbGateway, 0);

    // wait some ms to make sure internal slave is up and running
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // connect to internal Modbus slave
    mbExtMaster.setUp();

    // checks
    mbExtMaster.checkWriteReadRequestBits();
    mbExtMaster.checkWriteReadRequestRegisters();
    mbExtMaster.checkUnsupportedFunctionCode();

    // wait some ms to make sure internal slave is up and running
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // close connection
    mbExtMaster.tearDown();

    // join threads
    mbGatewayThread.join();
    mbExtSlaveThread.join();
}

TEST(TestModbusCommunication, checkConnectionTimeout_IntMasterExtSlave)
{
    // create external test fixtures
    FixtureExternalModbusMaster mbExtMaster;
    FixtureExternalModbusSlave mbExtSlave;

    // run external Modbus slave in extra thread
    std::thread mbExtSlaveThread(&FixtureExternalModbusSlave::setUp, &mbExtSlave);

    // wait some ms to make sure external slave is up and running
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // create Modbus gateway fixture
    FixtureModbusGateway mbGateway;

    // run gateway in extra thread -> no reconnection needed
    std::thread mbGatewayThread(&FixtureModbusGateway::setUp, &mbGateway, 0);

    // wait some ms to make sure internal slave is up and running
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // connect to internal Modbus slave
    mbExtMaster.setUp();

    // checks
    mbExtMaster.checkWriteReadRequestBits();

    // wait some ms to make sure internal slave is up and running
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // close connection
    mbExtMaster.tearDown();

    // join threads
    mbGatewayThread.join();
    mbExtSlaveThread.join();
}

}
