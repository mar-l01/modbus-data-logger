#include "integrationtests/fixtures/includes/FixtureDataLogger.hpp"

#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include "gtest/gtest.h"

#include <iostream>

namespace Fixture {

FixtureDataLogger::FixtureDataLogger(bool logOutputToConsole)
    : m_logOutputToConsole(logOutputToConsole)
{}

void FixtureDataLogger::logModbusRequest(const std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest)
{
    // print out request to capture it in console for logging test
    if (m_logOutputToConsole) {
        std::cout << mbRequest->convertToLogString();
    }
}

void FixtureDataLogger::logModbusResponse(const std::shared_ptr<Entity::ModbusTcpResponse>& mbResponse)
{
    // print out request to capture it in console for logging test
    if (m_logOutputToConsole) {
        std::cout << mbResponse->convertToLogString();
    }
}

}
