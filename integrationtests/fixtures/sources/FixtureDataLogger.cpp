#include "integrationtests/fixtures/includes/FixtureDataLogger.hpp"

#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include "gtest/gtest.h"

namespace Fixture {

FixtureDataLogger::FixtureDataLogger() {}

void FixtureDataLogger::logModbusRequest(const std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest) {}

void FixtureDataLogger::logModbusResponse(const std::shared_ptr<Entity::ModbusTcpResponse>& mbResponse) {}

}
