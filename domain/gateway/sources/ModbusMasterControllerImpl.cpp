#include "domain/gateway/includes/ModbusMasterControllerImpl.hpp"

#include "domain/entity/includes/ModbusTcpConstants.hpp"

namespace Gateway {

ModbusMasterControllerImpl::ModbusMasterControllerImpl(const std::shared_ptr<ModbusMaster>& mbMaster,
                                                       const std::string& ipAddr, const int port)
    : m_modbusMaster(mbMaster)
    , m_ipAddress(ipAddr)
    , m_port(port)
{}

void ModbusMasterControllerImpl::connect()
{
    m_modbusMaster->connect(m_ipAddress, m_port);
}

void ModbusMasterControllerImpl::setTimeout(const uint16_t timeoutInMs)
{
    m_modbusMaster->setResponseTimeout(timeoutInMs);
}

std::shared_ptr<Entity::ModbusTcpResponse> ModbusMasterControllerImpl::getExternalModbusSlaveResponse(
  std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest)
{
    // call respective method of Modbus master instance to obtain the reply
    auto modbusTcpResponse = callModbusMasterMethod(mbRequest);

    // assume that connection got lost if a timeout occurred
    if (modbusTcpResponse->getModbusOperationStatus() == Entity::ModbusOperationStatus::TIMEOUT) {
        // reconnect, but do not re-send request
        tryReconnecting();

        // TODO(Markus2101, 08.06.2020): Re-sending request would require to adjust transaction
        //      identifier for all following request/response pairs
        //      --> further discussion needed!
    }

    return modbusTcpResponse;
}

void ModbusMasterControllerImpl::disconnect()
{
    m_modbusMaster->close();
}

std::shared_ptr<Entity::ModbusTcpResponse> ModbusMasterControllerImpl::callModbusMasterMethod(
  std::shared_ptr<Entity::ModbusTcpRequest>& mbRequest)
{
    std::shared_ptr<Entity::ModbusTcpResponse> mbTcpResponse;

    switch (mbRequest->functionCode) {
        case static_cast<uint8_t>(Entity::ModbusFunctionCode::READ_COIL_VALUES): {
            auto mbResponse =
              m_modbusMaster->readCoilValues(mbRequest->getStartAddress(), mbRequest->getNumberOfValuesToReadOrWrite());

            // create response object with operation state and read values
            mbTcpResponse = std::make_shared<Entity::ModbusTcpResponse>(mbRequest, mbResponse.first);
            mbTcpResponse->setReadValues(mbResponse.second);

            break;
        }
        case static_cast<uint8_t>(Entity::ModbusFunctionCode::READ_DISCRETE_INPUT_VALUES): {
            auto mbResponse = m_modbusMaster->readDiscreteInputValues(mbRequest->getStartAddress(),
                                                                      mbRequest->getNumberOfValuesToReadOrWrite());

            // create response object with operation state and read values
            mbTcpResponse = std::make_shared<Entity::ModbusTcpResponse>(mbRequest, mbResponse.first);
            mbTcpResponse->setReadValues(mbResponse.second);

            break;
        }
        case static_cast<uint8_t>(Entity::ModbusFunctionCode::READ_HOLDING_REGISTER_VALUES): {
            auto mbResponse = m_modbusMaster->readHoldingRegisterValues(mbRequest->getStartAddress(),
                                                                        mbRequest->getNumberOfValuesToReadOrWrite());

            // create response object with operation state and read values
            mbTcpResponse = std::make_shared<Entity::ModbusTcpResponse>(mbRequest, mbResponse.first);
            mbTcpResponse->setReadValues(mbResponse.second);

            break;
        }
        case static_cast<uint8_t>(Entity::ModbusFunctionCode::READ_INPUT_REGISTER_VALUES): {
            auto mbResponse = m_modbusMaster->readInputRegisterValues(mbRequest->getStartAddress(),
                                                                      mbRequest->getNumberOfValuesToReadOrWrite());

            // create response object with operation state and read values
            mbTcpResponse = std::make_shared<Entity::ModbusTcpResponse>(mbRequest, mbResponse.first);
            mbTcpResponse->setReadValues(mbResponse.second);

            break;
        }
        case static_cast<uint8_t>(Entity::ModbusFunctionCode::WRITE_SINGLE_COIL_VALUE): {
            auto mbResponse =
              m_modbusMaster->writeSingleCoilValue(mbRequest->getStartAddress(), mbRequest->getSingleValueToWrite());

            // create response object with operation state only
            mbTcpResponse = std::make_shared<Entity::ModbusTcpResponse>(mbRequest, mbResponse);

            break;
        }
        case static_cast<uint8_t>(Entity::ModbusFunctionCode::WRITE_SINGLE_HOLDING_REGISTER_VALUE): {
            auto mbResponse = m_modbusMaster->writeSingleHoldingRegisterValue(mbRequest->getStartAddress(),
                                                                              mbRequest->getSingleValueToWrite());

            // create response object with operation state only
            mbTcpResponse = std::make_shared<Entity::ModbusTcpResponse>(mbRequest, mbResponse);

            break;
        }
        case static_cast<uint8_t>(Entity::ModbusFunctionCode::WRITE_MULTIPLE_COIL_VALUES): {
            auto mbResponse =
              m_modbusMaster->writeMultipleCoilValues(mbRequest->getStartAddress(), mbRequest->getCoilValuesToWrite());

            // create response object with operation state only
            mbTcpResponse = std::make_shared<Entity::ModbusTcpResponse>(mbRequest, mbResponse);

            break;
        }
        case static_cast<uint8_t>(Entity::ModbusFunctionCode::WRITE_MULTIPLE_HOLDING_REGISTER_VALUES): {
            auto mbResponse = m_modbusMaster->writeMultipleHoldingRegisterValues(
              mbRequest->getStartAddress(), mbRequest->getHoldingRegisterValuesToWrite());

            // create response object with operation state only
            mbTcpResponse = std::make_shared<Entity::ModbusTcpResponse>(mbRequest, mbResponse);

            break;
        }
        default:
            // currently nothing to be done if FC is not supported
            mbTcpResponse = std::make_shared<Entity::ModbusTcpResponse>(mbRequest, Entity::ModbusOperationStatus::FAIL);
            break;
    }

    return mbTcpResponse;
}

void ModbusMasterControllerImpl::tryReconnecting()
{
    // close connection if timeout was received due to another cause
    disconnect();
    m_modbusMaster->reconnect();
}

}
