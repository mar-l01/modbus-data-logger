#include "integrationtests/fixtures/includes/FixtureExternalModbusMaster.hpp"
#include "integrationtests/fixtures/includes/FixtureExternalModbusSlave.hpp"
#include "integrationtests/fixtures/includes/FixtureModbusDataLoggerFacade.hpp"
#include "integrationtests/fixtures/includes/TestConstants.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <fstream>
#include <stdio.h>
#include <thread>

namespace {

using namespace testing;
using namespace Fixture;
using Application::ApplicationState;

class TestModbusDataLoggerFacade
    : public Test
    , public FixtureModbusDataLoggerFacade
{
public:
    TestModbusDataLoggerFacade()
    {
        setUp();
    }

    ~TestModbusDataLoggerFacade()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

protected:
    void checkLoggedContent()
    {
        std::string expectedOutputWriteRequest(
          "--- Modbus Request ---> "
          "\n<0001><0000><000d><ff><10><00><00><00><03><06><12><34><56><78><9a><bc>\n\tTransaction Id: 1\n\tProtocol "
          "Id: 0 (TCP/IP Protocol)\n\tLength: 13\n\tUnit Id: 255\n\tFunction Code: "
          "WRITE_MULTIPLE_HOLDING_REGISTER_VALUES\n\tData Bytes:\n\t|--Start Address: 0\n\t|--Number of values to "
          "write: 3\n\t|--Number of bytes of values to write: 6\n\t|--Holding Register value (1): 4660\n\t|--Holding "
          "Register value (2): 22136\n\t|--Holding Register value (3): 39612\n");

        std::string expectedOutputWriteResponse(
          "<--- Modbus Response --- \n<0001><0000><0006><ff><10><00><00><00><03>\n\tTransaction Id: 1\n\tProtocol Id: "
          "0 (TCP/IP Protocol)\n\tLength: 6\n\tUnit Id: 255\n\tFunction Code: "
          "WRITE_MULTIPLE_HOLDING_REGISTER_VALUES\n\tData Bytes:\n\t|--Start Address: 0\n\t|--Number of values "
          "written: 3\n");

        std::string expectedOutputReadRequest(
          "--- Modbus Request ---> \n<0002><0000><0006><ff><03><00><00><00><03>\n\tTransaction Id: 2\n\tProtocol Id: 0 "
          "(TCP/IP Protocol)\n\tLength: 6\n\tUnit Id: 255\n\tFunction Code: READ_HOLDING_REGISTER_VALUES\n\tData "
          "Bytes:\n\t|--Start Address: 0\n\t|--Number of values to read: 3\n");

        std::string expectedOutputReadResponse(
          "<--- Modbus Response --- \n<0002><0000><0009><ff><03><06><12><34><56><78><9a><bc>\n\tTransaction Id: "
          "2\n\tProtocol Id: 0 (TCP/IP Protocol)\n\tLength: 9\n\tUnit Id: 255\n\tFunction Code: "
          "READ_HOLDING_REGISTER_VALUES\n\tData Bytes:\n\t|--Number of bytes to follow: 6\n\t|--Holding Register (1): "
          "4660\n\t|--Holding Register (2): 22136\n\t|--Holding Register (3): 39612\n");

        // check logged file
        std::ifstream logFile(FixtureTestConstants::LOG_FILE_PATH, std::ios::in);
        std::stringstream buffer;
        buffer << logFile.rdbuf();
        std::string fileContent = buffer.str();

        // check if log was successful (ignore exact date, just check Modbus data)
        EXPECT_TRUE(fileContent.find(expectedOutputWriteRequest) != std::string::npos);
        EXPECT_TRUE(fileContent.find(expectedOutputWriteResponse) != std::string::npos);
        EXPECT_TRUE(fileContent.find(expectedOutputReadRequest) != std::string::npos);
        EXPECT_TRUE(fileContent.find(expectedOutputReadResponse) != std::string::npos);

        // remove log file
        remove(FixtureTestConstants::LOG_FILE_PATH);
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

    // clean-up
    mbExtMaster.tearDown();
    stopComThread.join();
    mbExtSlaveThread.join();

    // then: facade is not running, not able to connect from external side
    bool expectConnectionFailure = true;
    mbExtMaster.setUp(expectConnectionFailure);
}

TEST_F(TestModbusDataLoggerFacade, startAndStopLogging)
{
    // ---------- start logging ----------
    // given: Modbus communication is running
    FixtureExternalModbusSlave mbExtSlave;
    std::thread mbExtSlaveThread(&FixtureExternalModbusSlave::setUp, &mbExtSlave);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    getFixture()->startModbusCommunication();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    FixtureExternalModbusMaster mbExtMaster;
    mbExtMaster.setUp();

    // when: start logging
    getFixture()->startLogger();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // then: communication is logged
    mbExtMaster.checkWriteReadRequestRegisters();

    // ---------- stop logging ----------
    // given: facade is running
    // when: stop logging, wait some time to be sure everything was logged until now
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    getFixture()->stopLogger();

    // then: communication is not logged anymore
    // check log file content after some time
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    checkLoggedContent();

    // clean-up
    std::thread stopComThread(&Application::ModbusDataLoggerFacade::stopModbusCommunication, getFixture());
    mbExtMaster.tearDown();
    stopComThread.join();
    mbExtSlaveThread.join();
}

TEST_F(TestModbusDataLoggerFacade, checkApplicationStateChanges)
{
    // ---------- start communication ----------
    // given: external Modbus slave running in extra thread
    FixtureExternalModbusSlave mbExtSlave;
    std::thread mbExtSlaveThread(&FixtureExternalModbusSlave::setUp, &mbExtSlave);

    // check default state is STOPPED
    const auto mbDataLoggerFacadeFixture = getFixture();
    EXPECT_EQ(mbDataLoggerFacadeFixture->getCurrentApplicationState(), ApplicationState::STOPPED);

    // register for application state changes
    StrictMock<MockFunction<void(const ApplicationState&)>> applicationStateListener;
    const auto applicationStateConnPtr =
      mbDataLoggerFacadeFixture->addApplicationStateListener(applicationStateListener.AsStdFunction());
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // when: start communication, wait some time to make sure internal slave is running -> STARTING -> STARTED
    {
        InSequence inSeq;
        EXPECT_CALL(applicationStateListener, Call(ApplicationState::STARTING));
        EXPECT_CALL(applicationStateListener, Call(ApplicationState::STARTED));
    }
    mbDataLoggerFacadeFixture->startModbusCommunication();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // then: external Modbus master is able to connect, full communication possible -> RUNNING
    FixtureExternalModbusMaster mbExtMaster;
    EXPECT_CALL(applicationStateListener, Call(ApplicationState::RUNNING));
    mbExtMaster.setUp();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // ---------- stop communication ----------
    // given: external Modbus components connected, facade is in RUNNING
    EXPECT_EQ(mbDataLoggerFacadeFixture->getCurrentApplicationState(), ApplicationState::RUNNING);

    // then: stop communication (use additional thread to not block main thread) -> STOPPING -> STOPPED
    {
        InSequence inSeq;
        EXPECT_CALL(applicationStateListener, Call(ApplicationState::STOPPING));
        EXPECT_CALL(applicationStateListener, Call(ApplicationState::STOPPED));
    }
    std::thread stopComThread(&Application::ModbusDataLoggerFacade::stopModbusCommunication, mbDataLoggerFacadeFixture);

    // clean-up
    mbExtMaster.tearDown();
    stopComThread.join();
    mbExtSlaveThread.join();

    // then: we are in stopped state
    EXPECT_EQ(mbDataLoggerFacadeFixture->getCurrentApplicationState(), ApplicationState::STOPPED);
}

}
