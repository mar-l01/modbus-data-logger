#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include "domain/entity/includes/ModbusTcpConstants.hpp"
#include "domain/entity/includes/ModbusTcpRequest.hpp"

#include <cmath>
#include <iomanip>

namespace Entity {

ModbusTcpResponse::ModbusTcpResponse()
    : m_mbRequest(std::make_shared<ModbusTcpRequest>())
    , m_operationStatus(ModbusOperationStatus::FAIL)
{}

ModbusTcpResponse::ModbusTcpResponse(const std::shared_ptr<ModbusTcpRequest>& mbRequest,
                                     const ModbusOperationStatus mbOpStatus)
    : m_mbRequest(mbRequest)
    , m_operationStatus(mbOpStatus)
{}

ModbusOperationStatus ModbusTcpResponse::getModbusOperationStatus() const
{
    return m_operationStatus;
}

std::vector<uint8_t> ModbusTcpResponse::getReadBitValues() const
{
    auto readValues = std::vector<uint8_t>();

    try {
        readValues = std::get<std::vector<uint8_t>>(m_readValues);
    } catch (std::bad_variant_access&) {
        // TODO(Markus2101, 25.05.2020): strategy about error handling
    }

    return readValues;
}

std::vector<uint16_t> ModbusTcpResponse::getReadRegisterValues() const
{
    auto readValues = std::vector<uint16_t>();

    try {
        readValues = std::get<std::vector<uint16_t>>(m_readValues);
    } catch (std::bad_variant_access& ex) {
        // TODO(Markus2101, 25.05.2020): strategy about error handling
    }

    return readValues;
}

void ModbusTcpResponse::setReadValues(const ModbusReadValues& vals)
{
    m_readValues = vals;
}

bool operator==(const ModbusTcpResponse& mbResA, const ModbusTcpResponse& mbResB)
{
    return mbResA.m_operationStatus == mbResB.m_operationStatus && mbResA.m_readValues == mbResB.m_readValues;
}

std::string ModbusTcpResponse::convertToLogString() const
{
    std::stringstream loggedString;
    loggedString << "<--- Modbus Response --- ";

    if (m_operationStatus != ModbusOperationStatus::SUCCESS) {
        // add bytes representation
        loggedString << '<' << std::hex << std::setw(4) << std::setfill('0') << m_mbRequest->transactionIdentifier;
        loggedString << "><" << std::hex << std::setw(4) << std::setfill('0') << m_mbRequest->protocolIdentifier;
        loggedString << "><" << std::hex << std::setw(4) << std::setfill('0') << m_mbRequest->lengthField;
        loggedString << "><" << std::hex << std::setw(2) << std::setfill('0')
                     << static_cast<int>(m_mbRequest->unitIdentifier);
        loggedString << "><" << std::hex << std::setw(2) << std::setfill('0')
                     << static_cast<int>(m_mbRequest->functionCode);
        // TODO(Markus2101, 2020-08-03): Convert read values into data bytes to be displayed here.
        //      Adapt length field above accordingly.

        // add named representation
        loggedString << "\n\tTransaction Id: " << std::dec << static_cast<int>(m_mbRequest->transactionIdentifier);
        loggedString << "\n\tProtocol Id: " << std::dec << static_cast<int>(m_mbRequest->protocolIdentifier)
                     << (m_mbRequest->protocolIdentifier == 0 ? " (TCP/IP Protocol)" : " ");
        loggedString << "\n\tLength: " << std::dec << static_cast<int>(m_mbRequest->lengthField);
        loggedString << "\n\tUnit Id: " << std::dec << static_cast<int>(m_mbRequest->unitIdentifier);
        loggedString << "\n\tFunction Code: "
                     << (ModbusFunctionCode::_from_integral(static_cast<int>(m_mbRequest->functionCode)))._to_string();

        std::string valueType("");
        switch (m_mbRequest->functionCode) {
            case static_cast<uint8_t>(ModbusFunctionCode::READ_COIL_VALUES):
                valueType = "Coil";
                break;
            case static_cast<uint8_t>(ModbusFunctionCode::READ_DISCRETE_INPUT_VALUES):
                valueType = "Discrete Input";
                break;
            case static_cast<uint8_t>(ModbusFunctionCode::READ_HOLDING_REGISTER_VALUES):
                valueType = "Holding Register";
                break;
            case static_cast<uint8_t>(ModbusFunctionCode::READ_INPUT_REGISTER_VALUES):
                valueType = "Input Register";
                break;
        }

        // add data bytes depending on FC
        loggedString << "\n\tData Bytes:";
        switch (m_mbRequest->functionCode) {
            case static_cast<uint8_t>(ModbusFunctionCode::READ_COIL_VALUES):
            case static_cast<uint8_t>(ModbusFunctionCode::READ_DISCRETE_INPUT_VALUES): {
                loggedString << "\n\t|--Number of bytes to follow: " << std::dec
                             << std::ceil(static_cast<int>(m_mbRequest->getNumberOfValuesToReadOrWrite()) / 8);

                int idx = 1;
                for (const auto dataByte : getReadBitValues()) {
                    loggedString << "\n\t|--" << valueType << " (" << idx << "): " << std::dec
                                 << static_cast<int>(dataByte);
                    ++idx;
                }
                break;
            }
            case static_cast<uint8_t>(ModbusFunctionCode::READ_HOLDING_REGISTER_VALUES):
            case static_cast<uint8_t>(ModbusFunctionCode::READ_INPUT_REGISTER_VALUES): {
                loggedString << "\n\t|--Number of bytes to follow: " << std::dec
                             << static_cast<int>(m_mbRequest->getNumberOfValuesToReadOrWrite()) * 2;

                int idx = 1;
                for (const auto dataByte : getReadRegisterValues()) {
                    loggedString << "\n\t|--" << valueType << " (" << idx << "): " << std::dec
                                 << static_cast<int>(dataByte);
                    ++idx;
                }
                break;
            }
            case static_cast<uint8_t>(ModbusFunctionCode::WRITE_SINGLE_COIL_VALUE):
            case static_cast<uint8_t>(ModbusFunctionCode::WRITE_SINGLE_HOLDING_REGISTER_VALUE):
                loggedString << "\n\t|--Start Address: " << std::dec
                             << static_cast<int>(m_mbRequest->getStartAddress());
                loggedString << "\n\t|--Single value to write: " << std::dec
                             << static_cast<int>(m_mbRequest->getSingleValueToWrite());

                break;

            case static_cast<uint8_t>(ModbusFunctionCode::WRITE_MULTIPLE_COIL_VALUES):
            case static_cast<uint8_t>(ModbusFunctionCode::WRITE_MULTIPLE_HOLDING_REGISTER_VALUES):
                loggedString << "\n\t|--Start Address: " << std::dec
                             << static_cast<int>(m_mbRequest->getStartAddress());
                loggedString << "\n\t|--Number of values written: " << std::dec
                             << static_cast<int>(m_mbRequest->getNumberOfValuesToReadOrWrite());

                break;
            default:
                // currently nothing to be done if FC is not supported
                loggedString << "\n\t|--Function code not supported!";

                break;
        }
    } else {
        loggedString << "\n\t" << (m_operationStatus == ModbusOperationStatus::FAIL ? "-- FAILED --" : "-- TIMEOUT --");
    }

    return loggedString.str();
}

}
