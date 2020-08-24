#pragma once

#include "application/interfaces/ModbusDataLoggerFacade.hpp"
#include <string>

namespace Gateway {
// forward declarations
class ModbusSlaveController;
class ModbusMasterController;
}

namespace Logging {
// forward declaration
class ModbusDataLogger;
}

namespace Application {

class ModbusDataLoggerFacadeImpl : public ModbusDataLoggerFacade
{
public:
    ModbusDataLoggerFacadeImpl(const std::string& mbConfigFilepath);

    /**
     * @see ModbusDataLoggerFacade::startModbusCommunication
     */
    void startModbusCommunication() override;

    /**
     * @see ModbusDataLoggerFacade::stopModbusCommunication
     */
    void stopModbusCommunication() override;

    /**
     * @see ModbusDataLoggerFacade::startLogger
     */
    void startLogger() override;

    /**
     * @see ModbusDataLoggerFacade::stopLogger
     */
    void stopLogger() override;

private:
    std::unique_ptr<Gateway::ModbusSlaveController> m_mbSlaveController;
    std::shared_ptr<Gateway::ModbusMasterController> m_mbMasterController;
    std::shared_ptr<Logging::ModbusDataLogger> m_dataLogger;

    void createModbusGateway(const std::string& mbConfigFilepath);
};

}
