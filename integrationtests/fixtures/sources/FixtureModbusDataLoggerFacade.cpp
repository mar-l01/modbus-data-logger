#include "integrationtests/fixtures/includes/FixtureModbusDataLoggerFacade.hpp"

#include "domain/application/includes/ModbusDataLoggerFacadeImpl.hpp"
#include "domain/entity/includes/ModbusLoggerConfiguration.hpp"
#include "domain/framework/includes/FileLoggerControllerFactory.hpp"
#include "domain/gateway/includes/ModbusComponentsFactory.hpp"
#include "domain/gateway/includes/ModbusGateway.hpp"
#include "domain/gateway/includes/ModbusMasterControllerImpl.hpp"
#include "domain/gateway/includes/ModbusSlaveControllerImpl.hpp"
#include "domain/logging/includes/ModbusDataLogger.hpp"
#include "domain/utility/includes/TimerFactory.hpp"
#include "domain/utility/interfaces/Timer.hpp"
#include "integrationtests/fixtures/includes/TestConstants.hpp"

namespace Fixture {

void FixtureModbusDataLoggerFacade::setUp()
{
    // helper to make sure, generated objects are not nullptr
    auto assertNotNullptr = [](const auto& interfacePointer, int line) {
        if (interfacePointer == nullptr) {
            throw std::string("[Line: " + std::to_string(line) + "]: Object is nullptr: Failed to create object!");
        }
    };

    // get Modbus master instance from factory
    auto mbMaster = Gateway::ModbusComponentsFactory::createModbusMaster(Gateway::ModbusComponentsFramework::LIBMODBUS);
    assertNotNullptr(mbMaster, __LINE__);

    // get Modbus slave instance from factory
    auto mbSlave = Gateway::ModbusComponentsFactory::createModbusSlave(Gateway::ModbusComponentsFramework::LIBMODBUS);
    assertNotNullptr(mbSlave, __LINE__);

    // create Modbus master controller
    auto mbMasterController = std::make_shared<Gateway::ModbusMasterControllerImpl>(
      mbMaster, FixtureTestConstants::MODBUS_IP_ADDRESS_EXTERNAL_SLAVE,
      FixtureTestConstants::MODBUS_PORT_EXTERNAL_SLAVE);
    mbMasterController->setTimeout(FixtureTestConstants::MODBUS_TIMEOUT_IN_MS);

    // create data logger
    auto fileLoggerController =
      Framework::FileLoggerControllerFactory::createFileLoggerController(Framework::LoggingFramework::SPDLOG);
    assertNotNullptr(fileLoggerController, __LINE__);
    auto dataLogger = std::make_shared<Logging::ModbusDataLogger>(fileLoggerController);
    assertNotNullptr(dataLogger, __LINE__);

    // set log file
    Entity::ModbusLoggerConfiguration mbLogConfig;
    mbLogConfig.logFilePath = FixtureTestConstants::LOG_FILE_PATH;
    dataLogger->changeLogFileConfiguration(mbLogConfig);

    // set up Modbus gateway
    auto mbGateway = std::make_shared<Gateway::ModbusGateway>(mbMasterController, dataLogger);
    assertNotNullptr(mbGateway, __LINE__);

    // create timer instance with a loop frequency of 1 ms
    std::shared_ptr<Utility::Timer> timerInstance = Utility::TimerFactory::createLoopTimer(1);
    assertNotNullptr(timerInstance, __LINE__);
    timerInstance->setTimeoutInMs(FixtureTestConstants::APPLICATION_TIMEOUT_IN_MS);

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

    // create Modbus slave controller and start it up
    auto mbSlaveController = std::make_shared<Gateway::ModbusSlaveControllerImpl>(
      mbSlave, mbGateway, timerInstance, mbDataMapping, FixtureTestConstants::MODBUS_IP_ADDRESS_INTERNAL_SLAVE,
      FixtureTestConstants::MODBUS_PORT_INTERNAL_SLAVE);
    assertNotNullptr(mbSlaveController, __LINE__);

    // create facade (cast required components to their interface type)
    m_mbDataLoggerFacade = std::make_shared<Application::ModbusDataLoggerFacadeImpl>(
      std::dynamic_pointer_cast<Gateway::ModbusMasterController>(mbMasterController),
      std::dynamic_pointer_cast<Gateway::ModbusSlaveController>(mbSlaveController), dataLogger, timerInstance);
    assertNotNullptr(m_mbDataLoggerFacade, __LINE__);
}

std::shared_ptr<Application::ModbusDataLoggerFacade> FixtureModbusDataLoggerFacade::getFixture() const
{
    return m_mbDataLoggerFacade;
}

} // namespace Fixture
