#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"
#include "domain/logging/includes/ModbusDataLogger.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using namespace ::testing;
using namespace Logging;

class TestModbusDataLogger : public ::testing::Test
{
protected:
    std::shared_ptr<ModbusDataLogger> createTestObject()
    {
        return std::make_shared<ModbusDataLogger>();
    }
};

TEST_F(TestModbusDataLogger, ctorSuccessful)
{
    EXPECT_NO_THROW(createTestObject());
}

TEST_F(TestModbusDataLogger, receiveSignalWithModbusRequest)
{
    auto testObj = createTestObject();

    MockFunction<void(const Entity::ModbusTcpRequest&)> signalCallback;
    auto connectionPointer = testObj->addModbusRequestListener(signalCallback.AsStdFunction());

    Entity::ModbusTcpRequest expectedModbusRequest;
    EXPECT_CALL(signalCallback, Call(expectedModbusRequest)).Times(1);
    testObj->logModbusRequest(expectedModbusRequest);

    // removing connection should not inform us about new data
    connectionPointer.reset();
    EXPECT_CALL(signalCallback, Call(expectedModbusRequest)).Times(0);
    testObj->logModbusRequest(expectedModbusRequest);
}

TEST_F(TestModbusDataLogger, receiveSignalWithModbusResponse)
{
    auto testObj = createTestObject();

    MockFunction<void(const Entity::ModbusTcpResponse&)> signalCallback;
    auto connectionPointer = testObj->addModbusResponseListener(signalCallback.AsStdFunction());

    Entity::ModbusTcpResponse expectedModbusResponse;
    EXPECT_CALL(signalCallback, Call(expectedModbusResponse)).Times(1);
    testObj->logModbusResponse(expectedModbusResponse);

    // removing connection should not inform us about new data
    connectionPointer.reset();
    EXPECT_CALL(signalCallback, Call(expectedModbusResponse)).Times(0);
    testObj->logModbusResponse(expectedModbusResponse);
}

}
