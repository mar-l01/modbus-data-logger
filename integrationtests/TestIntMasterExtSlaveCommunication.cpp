#include "integrationtests/fixtures/includes/FixtureExternalModbusSlave.hpp"
#include "integrationtests/fixtures/includes/FixtureModbusMasterController.hpp"

#include "gtest/gtest.h"

#include <chrono>
#include <thread>

namespace {

using namespace Fixture;

TEST(TestIntMasterExtSlaveCommunication, checkReadSingleRegister)
{
    FixtureExternalModbusSlave mbExtSlave;
    const int nbReqLoopIter = 1; // receive-response

    // set up external slave and run infinite request/response loop
    std::thread mbExtSlaveThread(&FixtureExternalModbusSlave::setUp, &mbExtSlave, nbReqLoopIter);

    // wait some ms to make sure slave is up and running
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // connect to external slave with master
    FixtureModbusMasterController mbMasterController;
    mbMasterController.setUp();

    // send Modbus request and check correct result
    mbMasterController.checkReadHoldingRegisterValue();

    // close connection to Modbus slave
    mbMasterController.tearDown();

    // join thread
    mbExtSlaveThread.join();
}

}
