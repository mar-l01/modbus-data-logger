#pragma once

#include "domain/entity/includes/ModbusTcpMessageFrame.hpp"

#include <memory>
#include <string>
#include <utility>
#include <variant>

namespace Entity {
class ModbusTcpRequest;

enum class ModbusOperationStatus
{
    SUCCESS,
    FAIL
};

template<typename T>
using ModbusReadOperationResult = std::pair<ModbusOperationStatus, std::vector<T>>;

class ModbusTcpResponse : public ModbusTcpMessageFrame
{
public:
    ModbusTcpResponse();
    ModbusTcpResponse(const ModbusOperationStatus mbOpStatus);

    uint8_t getNumberOfBytesOfReadValues() const;
    std::vector<uint8_t> getReadBitValues() const;
    std::vector<uint16_t> getReadRegisterValues() const;

    void setReadValues(const std::variant<std::vector<uint8_t>, std::vector<uint16_t>>& vals);

private:
    ModbusOperationStatus m_operationStatus;
    std::variant<std::vector<uint8_t>, std::vector<uint16_t>> m_readValues; // 1-bit or 16-bit values
};

}
