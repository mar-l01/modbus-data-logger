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
    void checkWriteReadModbusRequest();
    void tearDown();

private:
    std::unique_ptr<modbus_t, std::function<void(modbus_t*)>> m_modbusContext;
    std::unique_ptr<modbus_mapping_t, std::function<void(modbus_mapping_t*)>> m_modbusMapping;
};

}
