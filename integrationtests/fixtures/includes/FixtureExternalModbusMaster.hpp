#pragma once

#include "modbus/modbus.h"

#include <functional>
#include <memory>

namespace Fixture {

class FixtureExternalModbusMaster
{
public:
    FixtureExternalModbusMaster();

    void setUp();
    void tearDown();

    void checkWriteReadRequestBits();
    void checkWriteReadRequestRegisters();
    void checkUnsupportedFunctionCode();

private:
    std::unique_ptr<modbus_t, std::function<void(modbus_t*)>> m_modbusContext;
};

}
