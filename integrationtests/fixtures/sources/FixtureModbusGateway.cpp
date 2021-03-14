#include "integrationtests/fixtures/includes/FixtureModbusGateway.hpp"

#include "domain/gateway/includes/ModbusComponentsFactory.hpp"
#include "domain/gateway/includes/ModbusGateway.hpp"
#include "domain/gateway/includes/ModbusMasterControllerImpl.hpp"
#include "domain/gateway/includes/ModbusSlaveControllerImpl.hpp"
#include "domain/utility/includes/TimerFactory.hpp"
#include "domain/utility/interfaces/Timer.hpp"
#include "integrationtests/fixtures/includes/FixtureDataLogger.hpp"
#include "integrationtests/fixtures/includes/TestConstants.hpp"

#include "spdlog/spdlog.h"


namespace Fixture {

FixtureModbusGateway::FixtureModbusGateway(bool logOutputToConsole)
    : m_logOutputToConsole(logOutputToConsole)
    , m_isConnectionPossible(false)
{}

void FixtureModbusGateway::setUp(const int nbReconnections)
{
    using namespace Gateway;

    // get Modbus slave instance from factory and set up listening
    auto mbSlave = ModbusComponentsFactory::createModbusSlave(ModbusComponentsFramework::LIBMODBUS);

    // get Modbus master instance from factory
    auto mbMaster = ModbusComponentsFactory::createModbusMaster(ModbusComponentsFramework::LIBMODBUS);

    // create Modbus master controller and connect to external Modbusslave
    auto mbMasterController =
      std::make_shared<ModbusMasterControllerImpl>(mbMaster, FixtureTestConstants::MODBUS_IP_ADDRESS_EXTERNAL_SLAVE,
                                                   FixtureTestConstants::MODBUS_PORT_EXTERNAL_SLAVE);
    mbMasterController->connect();
    mbMasterController->setTimeout(FixtureTestConstants::MODBUS_TIMEOUT_IN_MS); // set timeout to 200 ms by default

    // set up Modbus gateway with a fixture representing the data logger
    auto mbGateway =
      std::make_shared<ModbusGateway>(mbMasterController, std::make_shared<FixtureDataLogger>(m_logOutputToConsole));

    // create Modbus data mapping
    Entity::ModbusDataMapping mbDataMapping;
    mbDataMapping.startAddressCoils = FixtureTestConstants::MODBUS_START_ADDRESS_COILS;
    mbDataMapping.startAddressDiscreteInputs = FixtureTestConstants::MODBUS_START_ADDRESS_DISCRETE_INPUTS;
    mbDataMapping.startAddressHoldingRegisters = FixtureTestConstants::MODBUS_START_ADDRESS_HOLDING_REGISTERS;
    mbDataMapping.startAddressInputRegisters = FixtureTestConstants::MODBUS_START_ADDRESS_INPUT_REGISTERS;
    mbDataMapping.nbCoils = FixtureTestConstants::MODBUS_NUMBER_COILS;
    mbDataMapping.nbDiscreteInputs = FixtureTestConstants::MODBUS_NUMBER_DISCRETE_INPUTS;
    mbDataMapping.nbHoldingRegisters = FixtureTestConstants::MODBUS_NUMBER_HOLDING_REGISTERS;
    mbDataMapping.nbInputRegisters = FixtureTestConstants::MODBUS_NUMBER_INPUT_REGISTERS;

    // create timer instance with a loop frequency of 1 ms
    int applicationTimeout = FixtureTestConstants::APPLICATION_TIMEOUT_IN_MS; // 2 seconds timeout
    std::atomic_bool timeoutStop = false;
    std::shared_ptr<Utility::Timer> timerInstance = Utility::TimerFactory::createLoopTimer(1);
    timerInstance->setTimeoutInMs(applicationTimeout);
    timerInstance->callOnTimeout([&timeoutStop]() {
        timeoutStop = true;
        SPDLOG_INFO("Timeout reached!");
    });

    // create Modbus slave controller
    auto mbSlaveController = std::make_unique<ModbusSlaveControllerImpl>(
      mbSlave, mbGateway, timerInstance, mbDataMapping, FixtureTestConstants::MODBUS_IP_ADDRESS_INTERNAL_SLAVE,
      FixtureTestConstants::MODBUS_PORT_INTERNAL_SLAVE);

    // run Modbus slave until 'stop' was received (no reconnection will be triggered then)
    int currentReconnections = 0;
    for (;;) {
        m_isConnectionPossible = true;
        mbSlaveController->waitForIncomingConnection();
        m_isConnectionPossible = false;

        mbSlaveController->run();

        if (currentReconnections == nbReconnections || timeoutStop) {
            break;
        }
        ++currentReconnections;
    }

    // stop timer
    timerInstance->stop();

    // close external connection in the end
    mbSlaveController->disconnect();
    mbMasterController->disconnect();
}

bool FixtureModbusGateway::isConnectionPossible() const
{
    return m_isConnectionPossible;
}

}
