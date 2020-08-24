#pragma once

#include "application/interfaces/ModbusDataLoggerFacade.hpp"
#include <string>

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
};

}
