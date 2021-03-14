#pragma once

#include "domain/entity/includes/ModbusDataMapping.hpp"

#include "modbus/modbus.h"

#include <functional>
#include <memory>
#include <tuple>

namespace Fixture {

// helpers to handle timeouts in tests
using timeoutTuple = std::tuple<uint8_t, uint16_t, uint>;
enum class TupleIndex
{
    FUNCTION_CODE,
    TIMEOUT_IN_MS,
    NUMBER_OF_TIMEOUTS
};

class FixtureExternalModbusSlave
{
public:
    FixtureExternalModbusSlave();
    FixtureExternalModbusSlave(const timeoutTuple& toTuple);

    void setUp();
    bool isConnectionPossible() const;

private:
    std::unique_ptr<modbus_t, std::function<void(modbus_t*)>> m_modbusContext;
    std::unique_ptr<modbus_mapping_t, std::function<void(modbus_mapping_t*)>> m_modbusMapping;
    int m_socket;
    bool m_isConnectionPossible;

    // {(fc, timeout, n)}
    // helper-tuple to delay a response n-times if function code fc is called for ms second
    timeoutTuple m_timeoutHelper;
    bool m_timeoutHelperActive;
    int m_curNbTimeouts;

    void setupModbusContext();
    void bind();
    void run();
    void setupAndFillModbusMapping();
};

}
