#pragma once

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace Entity {

enum class ModbusOperationStatus
{
    SUCCESS,
    FAIL,
    TIMEOUT
};

template<typename T>
using ModbusReadOperationResult = std::pair<ModbusOperationStatus, std::vector<T>>;

using ModbusReadValues = std::variant<std::vector<uint8_t>, std::vector<uint16_t>>;

class ModbusTcpResponse
{
public:
    ModbusTcpResponse();
    ModbusTcpResponse(const ModbusOperationStatus mbOpStatus);

    std::vector<uint8_t> getReadBitValues() const;
    std::vector<uint16_t> getReadRegisterValues() const;

    void setReadValues(const ModbusReadValues& vals);

private:
    ModbusOperationStatus m_operationStatus;
    ModbusReadValues m_readValues; // 1-bit or 16-bit values
};

}
