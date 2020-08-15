#pragma once

#include "domain/logging/interfaces/DataLogger.hpp"

namespace Entity {
// forward declarations
class ModbusTcpRequest;
class ModbusTcpResponse;
}

namespace Fixture {

class FixtureDataLogger : public virtual Logging::DataLogger
{
public:
    FixtureDataLogger(bool logOutputToConsole = false);

    void logModbusRequest(const std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest) override;
    void logModbusResponse(const std::shared_ptr<Entity::ModbusTcpResponse>& mbResponse) override;

private:
    bool m_logOutputToConsole;
};

}
