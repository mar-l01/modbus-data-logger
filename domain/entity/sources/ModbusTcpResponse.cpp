#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include "domain/entity/includes/ModbusTcpConstants.hpp"
#include "domain/entity/includes/ModbusTcpRequest.hpp"

#include <algorithm>
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
    loggedString << "<--- Modbus Response --- \n";

    if (m_operationStatus == ModbusOperationStatus::SUCCESS) {
        auto lengthFieldInBytes = computeLengthFieldInByte();

        // add bytes representation
        loggedString << '<' << std::hex << std::setw(4) << std::setfill('0') << m_mbRequest->transactionIdentifier;
        loggedString << "><" << std::hex << std::setw(4) << std::setfill('0') << m_mbRequest->protocolIdentifier;
        loggedString << "><" << std::hex << std::setw(4) << std::setfill('0') << lengthFieldInBytes;
        loggedString << "><" << std::hex << std::setw(2) << std::setfill('0')
                     << static_cast<int>(m_mbRequest->unitIdentifier);
        loggedString << "><" << std::hex << std::setw(2) << std::setfill('0')
                     << static_cast<int>(m_mbRequest->functionCode) << '>';
        for (const auto dataByte : getDataBytesInByteVector()) {
            loggedString << '<' << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(dataByte) << '>';
        }

        // add named representation
        loggedString << "\n\tTransaction Id: " << std::dec << static_cast<int>(m_mbRequest->transactionIdentifier);
        loggedString << "\n\tProtocol Id: " << std::dec << static_cast<int>(m_mbRequest->protocolIdentifier)
                     << (m_mbRequest->protocolIdentifier == 0 ? " (TCP/IP Protocol)" : " ");
        loggedString << "\n\tLength: " << std::dec << lengthFieldInBytes;
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
                             << std::ceil(static_cast<int>(m_mbRequest->getNumberOfValuesToReadOrWrite()) / 8.0);

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
        loggedString << "\t" << (m_operationStatus == ModbusOperationStatus::FAIL ? "-- FAILED --" : "-- TIMEOUT --");
    }

    loggedString << '\n'; // new line at the end

    return loggedString.str();
}

int ModbusTcpResponse::computeLengthFieldInByte() const
{
    int lengthField = 2; // FC and unit-id

    switch (m_mbRequest->functionCode) {
        case static_cast<uint8_t>(ModbusFunctionCode::READ_COIL_VALUES):
        case static_cast<uint8_t>(ModbusFunctionCode::READ_DISCRETE_INPUT_VALUES):
            // #bytes to follow + #bytes with single-bit values
            lengthField += (1 + std::ceil(getReadBitValues().size() / 8.0));
            break;
        case static_cast<uint8_t>(ModbusFunctionCode::READ_HOLDING_REGISTER_VALUES):
        case static_cast<uint8_t>(ModbusFunctionCode::READ_INPUT_REGISTER_VALUES):
            // #bytes to follow + (#bytes * 2) per register (16-bit)
            lengthField += (1 + getReadRegisterValues().size() * 2);
            break;
        case static_cast<uint8_t>(ModbusFunctionCode::WRITE_SINGLE_COIL_VALUE):
        case static_cast<uint8_t>(ModbusFunctionCode::WRITE_SINGLE_HOLDING_REGISTER_VALUE):
            // address (16-bit) + value (16-bit)
            lengthField += 4;
            break;
        case static_cast<uint8_t>(ModbusFunctionCode::WRITE_MULTIPLE_COIL_VALUES):
        case static_cast<uint8_t>(ModbusFunctionCode::WRITE_MULTIPLE_HOLDING_REGISTER_VALUES):
            // address (16-bit) + #values written (16-bit)
            lengthField += 4;
            break;
    }

    return lengthField;
}

std::vector<uint8_t> ModbusTcpResponse::getDataBytesInByteVector() const
{
    std::vector<uint8_t> bytesRepresentation;

    // adds a 16-bit value as two byte values to vector above
    auto add16BitValueToVector = [&bytesRepresentation](const uint16_t val) {
        bytesRepresentation.emplace_back(static_cast<uint8_t>(val >> 8));
        bytesRepresentation.emplace_back(static_cast<uint8_t>(val & 0x0000ffff));
    };

    switch (m_mbRequest->functionCode) {
        case static_cast<uint8_t>(ModbusFunctionCode::READ_COIL_VALUES):
        case static_cast<uint8_t>(ModbusFunctionCode::READ_DISCRETE_INPUT_VALUES): {
            // #bytes to follow, #bytes with single-bit values
            auto readBitVals = getReadBitValues();
            bytesRepresentation.emplace_back(std::ceil(readBitVals.size() / 8.0));

            // each bit represents one coil value, first requested coil is at LSB of first byte,
            // MSBs of last byte are stuffed with 0 if they do not represent a coil
            int currentBit = 0;
            uint8_t singleByte = 0;
            for (const auto singleBit : readBitVals) {
                singleByte |= (singleBit << currentBit);

                // jump to next byte and reset bit counter or move to next bit
                if (currentBit == 7) {
                    currentBit = 0;
                    bytesRepresentation.emplace_back(singleByte);
                    singleByte = 0; // reset byte for next byte to be computed
                } else {
                    ++currentBit;
                }
            }
            if (currentBit != 0) {
                // at last byte which was not fully filled to vector
                bytesRepresentation.emplace_back(singleByte);
            }
            break;
        }
        case static_cast<uint8_t>(ModbusFunctionCode::READ_HOLDING_REGISTER_VALUES):
        case static_cast<uint8_t>(ModbusFunctionCode::READ_INPUT_REGISTER_VALUES): {
            // #bytes to follow, (#bytes * 2) per register (16-bit)
            auto readRegVals = getReadRegisterValues();
            bytesRepresentation.emplace_back(static_cast<uint8_t>(readRegVals.size() * 2));

            for (const auto regVal : readRegVals) {
                add16BitValueToVector(regVal);
            }
            break;
        }
        case static_cast<uint8_t>(ModbusFunctionCode::WRITE_SINGLE_COIL_VALUE):
        case static_cast<uint8_t>(ModbusFunctionCode::WRITE_SINGLE_HOLDING_REGISTER_VALUE):
            // address (16-bit), value (16-bit)
            add16BitValueToVector(m_mbRequest->getStartAddress());
            add16BitValueToVector(m_mbRequest->getSingleValueToWrite());
            break;
        case static_cast<uint8_t>(ModbusFunctionCode::WRITE_MULTIPLE_COIL_VALUES):
            // address (16-bit), #values written (16-bit)
            add16BitValueToVector(m_mbRequest->getStartAddress());
            add16BitValueToVector(m_mbRequest->getCoilValuesToWrite().size());
            break;
        case static_cast<uint8_t>(ModbusFunctionCode::WRITE_MULTIPLE_HOLDING_REGISTER_VALUES):
            // address (16-bit), #values written (16-bit)
            add16BitValueToVector(m_mbRequest->getStartAddress());
            add16BitValueToVector(m_mbRequest->getHoldingRegisterValuesToWrite().size());
            break;
    }

    return bytesRepresentation;
}

}
