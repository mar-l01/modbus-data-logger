#include "integrationtests/fixtures/includes/FixtureExternalModbusSlave.hpp"

#include "domain/entity/includes/ModbusTcpConstants.hpp"
#include "integrationtests/fixtures/includes/TestConstants.hpp"

#include "gtest/gtest.h"

#include <thread>
#include <unistd.h>

namespace Fixture {

FixtureExternalModbusSlave::FixtureExternalModbusSlave()
    : m_socket(-1)
    , m_timeoutHelperActive(false)
    , m_curNbTimeouts(0)
{}

FixtureExternalModbusSlave::FixtureExternalModbusSlave(const timeoutTuple& toTuple)
    : m_socket(-1)
    , m_timeoutHelper(toTuple)
    , m_timeoutHelperActive(true)
    , m_curNbTimeouts(0)
{}

void FixtureExternalModbusSlave::setUp()
{
    // create Modbus context
    setupModbusContext();

    // fill Modbus mapping with values
    setupAndFillModbusMapping();

    // set up listening and wait for incoming connection
    bind();

    // run Modbus loop
    run();

    // close connection
    modbus_close(m_modbusContext.get());
}

void FixtureExternalModbusSlave::setupModbusContext()
{
    // clang-format off
    m_modbusContext = std::move(std::unique_ptr<modbus_t, std::function<void(modbus_t*)>>(
        modbus_new_tcp(FixtureTestConstants::MODBUS_IP_ADDRESS_EXTERNAL_SLAVE,
            FixtureTestConstants::MODBUS_PORT_EXTERNAL_SLAVE),
                [this](modbus_t* mbCtx) {
                    // use custom-deleter provided by libmodbus
                    modbus_free(mbCtx);
                }));
    // clang-format on

    // make sure context is not null
    ASSERT_NE(m_modbusContext, nullptr);
}

void FixtureExternalModbusSlave::bind()
{
    // listen for a single incoming connection
    m_socket = modbus_tcp_listen(m_modbusContext.get(), 1);

    // make sure context is not null
    ASSERT_NE(m_socket, -1);

    // accept incoming connection
    modbus_tcp_accept(m_modbusContext.get(), &m_socket);
}

void FixtureExternalModbusSlave::run()
{
    int reqLen = 0;
    auto modbusRequest = std::vector<uint8_t>(MODBUS_TCP_MAX_ADU_LENGTH);

    // infinite request loop
    for (;;) {
        do {
            reqLen = modbus_receive(m_modbusContext.get(), modbusRequest.data());
        } while (reqLen == 0); // 0:= indication request ignored

        // error in receiving request
        if (reqLen == -1) {
            // Modbus master closed connection (no error)
            if (errno == 104) {
                break;
            }

            // else failing error
            FAIL();
            break;
        }

        // helper for testing timeouts of internal Master to external slave
        // tests n timeouts of function code fc with a timeout interval of ms seconds
        if (m_timeoutHelperActive) {
            if (modbusRequest[Entity::ModbusMessageFrameByte::FUNCTION_CODE] ==
                std::get<static_cast<int>(TupleIndex::FUNCTION_CODE)>(m_timeoutHelper)) {
                std::this_thread::sleep_for(
                  std::chrono::milliseconds(std::get<static_cast<int>(TupleIndex::TIMEOUT_IN_MS)>(m_timeoutHelper)));
                ++m_curNbTimeouts;
            }

            if (m_curNbTimeouts == std::get<static_cast<int>(TupleIndex::NUMBER_OF_TIMEOUTS)>(m_timeoutHelper)) {
                m_timeoutHelperActive = false;
            }
        }

        modbus_reply(m_modbusContext.get(), modbusRequest.data(), reqLen, m_modbusMapping.get());

        // error in sending response
        if (reqLen == -1) {
            FAIL();
            break;
        }
    }

    // close socket after connection (necessary to close file descriptor)
    if (close(m_socket) == 0) {
        // reset socket value
        m_socket = -1;
    } else {
        FAIL();
    }
}

void FixtureExternalModbusSlave::setupAndFillModbusMapping()
{
    // clang-format off
    m_modbusMapping = std::move(std::unique_ptr<modbus_mapping_t, std::function<void(modbus_mapping_t*)>>(modbus_mapping_new_start_address(
        FixtureTestConstants::MODBUS_START_ADDRESS_COILS, FixtureTestConstants::MODBUS_NUMBER_COILS,
        FixtureTestConstants::MODBUS_START_ADDRESS_DISCRETE_INPUTS, FixtureTestConstants::MODBUS_NUMBER_DISCRETE_INPUTS,
        FixtureTestConstants::MODBUS_START_ADDRESS_HOLDING_REGISTERS, FixtureTestConstants::MODBUS_NUMBER_HOLDING_REGISTERS,
        FixtureTestConstants::MODBUS_START_ADDRESS_INPUT_REGISTERS, FixtureTestConstants::MODBUS_NUMBER_INPUT_REGISTERS),
        [this](modbus_mapping_t* mbMapping){
            // use custom-deleter provided by libmodbus
            modbus_mapping_free(mbMapping);
        }));
    // clang-format on

    // make sure context is not null
    ASSERT_NE(m_modbusContext, nullptr);

    // fill Modbus mapping with default values
    for (int i = 0; i < m_modbusMapping->nb_bits; ++i) {
        m_modbusMapping->tab_bits[i] = (i % 2);
    }

    for (int i = 0; i < m_modbusMapping->nb_input_bits; ++i) {
        m_modbusMapping->tab_input_bits[i] = (i % 2);
    }

    for (int i = 0; i < m_modbusMapping->nb_registers; ++i) {
        m_modbusMapping->tab_registers[i] = 0x1111 + i;
    }

    for (int i = 0; i < m_modbusMapping->nb_input_registers; ++i) {
        m_modbusMapping->tab_input_registers[i] = 0x2222 + i;
    }
}

}
