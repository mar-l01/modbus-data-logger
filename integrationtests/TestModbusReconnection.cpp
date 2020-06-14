#include "integrationtests/fixtures/includes/FixtureExternalModbusMaster.hpp"
#include "integrationtests/fixtures/includes/FixtureExternalModbusSlave.hpp"
#include "integrationtests/fixtures/includes/FixtureModbusGateway.hpp"

#include "gtest/gtest.h"

#include <chrono>
#include <thread>

namespace {

using namespace Fixture;

class TestModbusReconnection : public ::testing::TestWithParam<int>
{
protected:
    void reconnect(const int nbReconnections)
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

        // run gateway in extra thread -> allow for no reconnection
        std::thread mbGatewayThread(&FixtureModbusGateway::setUp, &mbGateway, nbReconnections);

        // reconnect to internal slave again and again
        int currentReconnections = 0;
        for (;;) {
            // wait some ms to make sure internal slave is up and running
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            // connect to internal Modbus slave
            mbExtMaster.setUp();

            // wait some ms to make sure internal slave is up and running
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            // close connection
            mbExtMaster.tearDown();

            // break infinite reconnection loop
            if (currentReconnections == nbReconnections) {
                break;
            }
            ++currentReconnections;
        }

        // join threads
        mbGatewayThread.join();
        mbExtSlaveThread.join();
    }
};

TEST_P(TestModbusReconnection, reconnect_n_times)
{
    int nbReconnections = GetParam();

    EXPECT_NO_FATAL_FAILURE(reconnect(nbReconnections));
}

INSTANTIATE_TEST_CASE_P(ModbusReconnectionTests, TestModbusReconnection, ::testing::Values(0, 1, 2));

}
