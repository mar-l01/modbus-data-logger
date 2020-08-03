#include "domain/entity/includes/ModbusTcpRequest.hpp"

#include "domain/entity/includes/ModbusTcpConstants.hpp"

#include <iomanip>

namespace Entity {

ModbusTcpRequest::ModbusTcpRequest() {}

ModbusTcpRequest::ModbusTcpRequest(const std::vector<uint8_t>& mbTcpReq)
{
    initFromByteVector(mbTcpReq);
}

uint8_t ModbusTcpRequest::getNumberOfBytesOfValuesToWrite() const
{
    // allowed FCs: 0x0f, 0x10
    // number of bytes to write (multiple coils or registers) := byte 4 of data-bytes
    return dataBytes[4];
}

std::vector<uint8_t> ModbusTcpRequest::getCoilValuesToWrite() const
{
    // allowed FC: 0x0f (start byte 5)
    return extractBitValues(ModbusDataByteOffset::WRITE_VALUES, getNumberOfValuesToReadOrWrite());
}

std::vector<uint16_t> ModbusTcpRequest::getHoldingRegisterValuesToWrite() const
{
    // allowed FC: 0x10 (start byte 5)
    return extractRegisterValues(ModbusDataByteOffset::WRITE_VALUES, getNumberOfValuesToReadOrWrite());
}

bool operator==(const ModbusTcpRequest& mbReqA, const ModbusTcpRequest& mbReqB)
{
    return mbReqA.transactionIdentifier == mbReqB.transactionIdentifier &&
           mbReqA.protocolIdentifier == mbReqB.protocolIdentifier && mbReqA.lengthField == mbReqB.lengthField &&
           mbReqA.unitIdentifier == mbReqB.unitIdentifier && mbReqA.functionCode == mbReqB.functionCode;
}

std::string ModbusTcpRequest::convertToLogString() const
{
    std::stringstream loggedString;
    loggedString << "--- Modbus Request ---> ";

    // add bytes representation
    loggedString << '<' << std::hex << std::setw(4) << std::setfill('0') << transactionIdentifier;
    loggedString << "><" << std::hex << std::setw(4) << std::setfill('0') << protocolIdentifier;
    loggedString << "><" << std::hex << std::setw(4) << std::setfill('0') << lengthField;
    loggedString << "><" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(unitIdentifier);
    loggedString << "><" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(functionCode);
    for (const auto dataByte : dataBytes) {
        loggedString << '<' << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(dataByte) << '>';
    }

    // add named representation
    loggedString << "\n\tTransaction Id: " << std::dec << static_cast<int>(transactionIdentifier);
    loggedString << "\n\tProtocol Id: " << std::dec << static_cast<int>(protocolIdentifier)
                 << (protocolIdentifier == 0 ? " (TCP/IP Protocol)" : " ");
    loggedString << "\n\tLength: " << std::dec << static_cast<int>(lengthField);
    loggedString << "\n\tUnit Id: " << std::dec << static_cast<int>(unitIdentifier);
    loggedString << "\n\tFunction Code: "
                 << (ModbusFunctionCode::_from_integral(static_cast<int>(functionCode)))._to_string();

    // add data bytes depending on FC
    loggedString << "\n\tData Bytes:";
    loggedString << "\n\t|--Start Address: " << std::dec << static_cast<int>(getStartAddress());
    switch (functionCode) {
        case static_cast<uint8_t>(ModbusFunctionCode::READ_COIL_VALUES):
        case static_cast<uint8_t>(ModbusFunctionCode::READ_DISCRETE_INPUT_VALUES):
        case static_cast<uint8_t>(ModbusFunctionCode::READ_HOLDING_REGISTER_VALUES):
        case static_cast<uint8_t>(ModbusFunctionCode::READ_INPUT_REGISTER_VALUES):
            loggedString << "\n\t|--Number of values to read: " << std::dec
                         << static_cast<int>(getNumberOfValuesToReadOrWrite());
            break;

        case static_cast<uint8_t>(ModbusFunctionCode::WRITE_SINGLE_COIL_VALUE):
        case static_cast<uint8_t>(ModbusFunctionCode::WRITE_SINGLE_HOLDING_REGISTER_VALUE):
            loggedString << "\n\t|--Single value to write: " << std::dec << static_cast<int>(getSingleValueToWrite());

            break;

        case static_cast<uint8_t>(ModbusFunctionCode::WRITE_MULTIPLE_COIL_VALUES): {
            loggedString << "\n\t|--Number of values to write: " << std::dec
                         << static_cast<int>(getNumberOfValuesToReadOrWrite());
            loggedString << "\n\t|--Number of bytes of values to write: "
                         << static_cast<int>(getNumberOfBytesOfValuesToWrite());

            int idx = 1;
            for (const auto dataByte : getCoilValuesToWrite()) {
                loggedString << "\n\t|--Coil value (" << idx << "): " << std::dec << static_cast<int>(dataByte);
                ++idx;
            }

            break;
        }
        case static_cast<uint8_t>(ModbusFunctionCode::WRITE_MULTIPLE_HOLDING_REGISTER_VALUES): {
            loggedString << "\n\t|--Number of values to write: " << std::dec
                         << static_cast<int>(getNumberOfValuesToReadOrWrite());
            loggedString << "\n\t|--Number of bytes of values to write: "
                         << static_cast<int>(getNumberOfBytesOfValuesToWrite());

            int idx = 1;
            for (const auto dataByte : getHoldingRegisterValuesToWrite()) {
                loggedString << "\n\t|--Holding Register value (" << idx << "): " << std::dec
                             << static_cast<int>(dataByte);
                ++idx;
            }

            break;
        }
        default:
            // currently nothing to be done if FC is not supported
            loggedString << "\n\t|--Function code not supported!";

            break;
    }

    return loggedString.str();
}

}
