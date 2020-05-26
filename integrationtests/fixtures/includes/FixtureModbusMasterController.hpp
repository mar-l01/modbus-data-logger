#pragma once

#include "domain/gateway/includes/ModbusMasterController.hpp"
#include "domain/gateway/interfaces/ModbusMaster.hpp"

namespace Fixture {

class FixtureModbusMasterController
{
public:
    FixtureModbusMasterController();

    void setUp();
    void checkReadHoldingRegisterValue();
    void tearDown();

private:
    std::unique_ptr<Gateway::ModbusMaster> m_modbusMaster;
};

}
