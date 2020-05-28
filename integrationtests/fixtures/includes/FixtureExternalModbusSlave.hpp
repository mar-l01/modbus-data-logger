#pragma once

#include "domain/entity/includes/ModbusDataMapping.hpp"

#include "modbus/modbus.h"

#include <functional>
#include <memory>

namespace Fixture {

class FixtureExternalModbusSlave
{
public:
    FixtureExternalModbusSlave();

    void setUp();

private:
    std::unique_ptr<modbus_t, std::function<void(modbus_t*)>> m_modbusContext;
    std::unique_ptr<modbus_mapping_t, std::function<void(modbus_mapping_t*)>> m_modbusMapping;
    int m_socket;

    void setupModbusContext();
    void bind();
    void run();
    void setupAndFillModbusMapping();
};

}
