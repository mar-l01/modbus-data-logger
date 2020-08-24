#pragma once

#include "domain/application/interfaces/ModbusDataLoggerFacade.hpp"

#include <memory>
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
    ModbusDataLoggerFacadeImpl(const std::shared_ptr<Gateway::ModbusMasterController>& mbMasterController,
                               const std::shared_ptr<Gateway::ModbusSlaveController>& mbSlaveController,
                               const std::shared_ptr<Logging::ModbusDataLogger>& dataLogger);

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
    std::shared_ptr<Gateway::ModbusMasterController> m_mbMasterController;
    std::shared_ptr<Gateway::ModbusSlaveController> m_mbSlaveController;
    std::shared_ptr<Logging::ModbusDataLogger> m_dataLogger;
};

}
