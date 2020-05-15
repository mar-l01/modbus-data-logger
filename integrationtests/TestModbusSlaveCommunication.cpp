#include "integrationtests/fixtures/includes/FixtureExternalModbusMaster.hpp"
#include "integrationtests/fixtures/includes/FixtureModbusSlaveController.hpp"

#include "gtest/gtest.h"

#include <chrono>
#include <thread>

namespace {

using namespace Fixture;

TEST(TestModbusSlaveCommunication, checkReadWriteSingleRegister)
{
    FixtureModbusSlaveController mbSlaveController;
    FixtureExternalModbusMaster mbTestMaster;
    const int nbReqLoopIter = 2; // receive-response|receive-response

    // set up slave and run infinite request/response loop
    std::thread mbSlaveThread(&FixtureModbusSlaveController::setUp, &mbSlaveController, nbReqLoopIter);

    // wait some ms to make sure connection is up
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // connect to slave controller from master
    mbTestMaster.setUp();

    // wait some ms to make sure receive/response loop is running
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // send Modbus request and check correct result
    mbTestMaster.checkWriteReadModbusRequest();

    // close connection to Modbus slave
    mbTestMaster.tearDown();

    // join thread
    mbSlaveThread.join();
}

}
