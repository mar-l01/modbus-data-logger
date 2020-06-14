#include "integrationtests/fixtures/includes/FixtureModbusGateway.hpp"

#include "domain/gateway/includes/ModbusComponentsFactory.hpp"
#include "domain/gateway/includes/ModbusGateway.hpp"
#include "domain/gateway/includes/ModbusMasterController.hpp"
#include "domain/gateway/includes/ModbusSlaveController.hpp"
#include "domain/utility/includes/TimerImpl.hpp"
#include "domain/utility/interfaces/Timer.hpp"
#include "integrationtests/fixtures/includes/TestConstants.hpp"

namespace Fixture {

FixtureModbusGateway::FixtureModbusGateway() {}

void FixtureModbusGateway::setUp(const int nbReconnections)
{
    using namespace Gateway;

    // get Modbus slave instance from factory and set up listening
    auto mbSlave = ModbusComponentsFactory::createModbusSlave(ModbusComponentsFramework::LIBMODBUS);

    // get Modbus master instance from factory
    auto mbMaster = ModbusComponentsFactory::createModbusMaster(ModbusComponentsFramework::LIBMODBUS);

    // create Modbus master controller and connect to external Modbusslave
    auto mbMasterController =
      std::make_shared<ModbusMasterController>(mbMaster, FixtureTestConstants::MODBUS_IP_ADDRESS_EXTERNAL_SLAVE,
                                               FixtureTestConstants::MODBUS_PORT_EXTERNAL_SLAVE);
    mbMasterController->connect();
    mbMasterController->setTimeout(200); // set timeout to 200 ms by default

    // set up Modbus gateway
    auto mbGateway = std::make_shared<ModbusGateway>(mbMasterController);

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

    // create timer instance
    std::shared_ptr<Utility::Timer> timerInstance = std::make_shared<Utility::TimerImpl>();
    // TODO(Markus2101, 14.06.2020): set callback if timeout is reached

    // create Modbus slave controller
    auto mbSlaveController = std::make_unique<ModbusSlaveController>(
      mbSlave, mbGateway, timerInstance, mbDataMapping, FixtureTestConstants::MODBUS_IP_ADDRESS_INTERNAL_SLAVE,
      FixtureTestConstants::MODBUS_PORT_INTERNAL_SLAVE);

    // run Modbus slave until 'stop' was received (no reconnection will be triggered then)
    int currentReconnections = 0;
    for (;;) {
        mbSlaveController->waitForIncomingConnection();
        mbSlaveController->run();

        if (currentReconnections == nbReconnections) {
            break;
        }
        ++currentReconnections;
    }

    // close external connection in the end
    mbSlaveController->closeConnection();
    mbMasterController->closeConnection();
}

}
