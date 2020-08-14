#pragma once

#include "modbus/modbus.h"

#include <functional>
#include <memory>

namespace Fixture {

class FixtureExternalModbusMaster
{
public:
    FixtureExternalModbusMaster();

    void setUp(bool expectConnectionFailure = false);
    void tearDown();

    void checkWriteReadRequestBits();
    void checkWriteReadRequestRegisters();
    void checkUnsupportedFunctionCode();

    void checkResponseTimeoutReadHoldingRegisters();

    void checkLogging();

private:
    std::unique_ptr<modbus_t, std::function<void(modbus_t*)>> m_modbusContext;
};

}
