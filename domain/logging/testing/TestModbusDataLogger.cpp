#include "domain/entity/includes/ModbusLoggerConfiguration.hpp"
#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"
#include "domain/logging/includes/ModbusDataLogger.hpp"
#include "domain/logging/testing/gmock/MockFileLoggerController.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <thread>

namespace {

using namespace ::testing;
using namespace Logging;

class TestModbusDataLogger : public ::testing::Test
{
protected:
    TestModbusDataLogger()
        : m_fileLoggerControllerMock(std::make_shared<MockFileLoggerController>())
    {}

    std::shared_ptr<ModbusDataLogger> createTestObject()
    {
        return std::make_shared<ModbusDataLogger>(m_fileLoggerControllerMock);
    }

    std::shared_ptr<MockFileLoggerController> m_fileLoggerControllerMock;
};

TEST_F(TestModbusDataLogger, ctorSuccessful)
{
    EXPECT_NO_THROW(createTestObject());
}

TEST_F(TestModbusDataLogger, logModbusRequest)
{
    auto testObj = createTestObject();

    MockFunction<void(const Entity::ModbusTcpRequest&)> signalCallback;
    auto connectionPointer = testObj->addModbusRequestListener(signalCallback.AsStdFunction());

    Entity::ModbusTcpRequest expectedModbusRequest;
    std::variant<Entity::ModbusTcpRequest, Entity::ModbusTcpResponse> receivedModbusRequest;
    EXPECT_CALL(signalCallback, Call(expectedModbusRequest)).Times(1);
    EXPECT_CALL(*m_fileLoggerControllerMock, logModbusData(_)).WillOnce(SaveArg<0>(&receivedModbusRequest));
    testObj->logModbusRequest(expectedModbusRequest);

    // wait some time until expected function was called (runs in detached thread)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(std::get<Entity::ModbusTcpRequest>(receivedModbusRequest), expectedModbusRequest);

    // removing connection should not inform us about new data, i.e. not raise a new signal
    connectionPointer.reset();
    EXPECT_CALL(signalCallback, Call(expectedModbusRequest)).Times(0);
    EXPECT_CALL(*m_fileLoggerControllerMock, logModbusData(_)).Times(1);
    testObj->logModbusRequest(expectedModbusRequest);

    // wait some time until expected function was called (runs in detached thread)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

TEST_F(TestModbusDataLogger, logModbusResponse)
{
    auto testObj = createTestObject();

    MockFunction<void(const Entity::ModbusTcpResponse&)> signalCallback;
    auto connectionPointer = testObj->addModbusResponseListener(signalCallback.AsStdFunction());

    Entity::ModbusTcpResponse expectedModbusResponse;
    std::variant<Entity::ModbusTcpRequest, Entity::ModbusTcpResponse> receivedModbusResponse;
    EXPECT_CALL(signalCallback, Call(expectedModbusResponse)).Times(1);
    EXPECT_CALL(*m_fileLoggerControllerMock, logModbusData(_)).WillOnce(SaveArg<0>(&receivedModbusResponse));
    testObj->logModbusResponse(expectedModbusResponse);

    // wait some time until expected function was called (runs in detached thread)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(std::get<Entity::ModbusTcpResponse>(receivedModbusResponse), expectedModbusResponse);

    // removing connection should not inform us about new data, i.e. not raise a new signal
    connectionPointer.reset();
    EXPECT_CALL(signalCallback, Call(expectedModbusResponse)).Times(0);
    EXPECT_CALL(*m_fileLoggerControllerMock, logModbusData(_)).Times(1);
    testObj->logModbusResponse(expectedModbusResponse);

    // wait some time until expected function was called (runs in detached thread)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

TEST_F(TestModbusDataLogger, startLogging)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_fileLoggerControllerMock, startLogger()).Times(1);
    testObj->startLogging();
}

TEST_F(TestModbusDataLogger, stopLogging)
{
    auto testObj = createTestObject();

    EXPECT_CALL(*m_fileLoggerControllerMock, stopLogger()).Times(1);
    testObj->stopLogging();
}

TEST_F(TestModbusDataLogger, changeLogFileConfiguration)
{
    auto testObj = createTestObject();

    // set-up configuration file
    Entity::ModbusLoggerConfiguration mbLogConfig;
    mbLogConfig.logFilePath = "test-path";
    mbLogConfig.loggerName = "test-logger";
    mbLogConfig.maxLogFileSizeInMb = 4;
    mbLogConfig.maxNumberOfFiles = 2;

    EXPECT_CALL(*m_fileLoggerControllerMock, setLogConfiguration(mbLogConfig)).Times(1);
    testObj->changeLogFileConfiguration(mbLogConfig);
}

}
