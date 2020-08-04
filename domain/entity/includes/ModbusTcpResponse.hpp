#pragma once

#include "domain/entity/interfaces/ModbusDataLog.hpp"

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace Entity {

// forward declaration
class ModbusTcpRequest;

enum class ModbusOperationStatus
{
    SUCCESS,
    FAIL,
    TIMEOUT
};

template<typename T>
using ModbusReadOperationResult = std::pair<ModbusOperationStatus, std::vector<T>>;

using ModbusReadValues = std::variant<std::vector<uint8_t>, std::vector<uint16_t>>;

class ModbusTcpResponse : public ModbusDataLog
{
public:
    ModbusTcpResponse();
    ModbusTcpResponse(const std::shared_ptr<ModbusTcpRequest>& mbRequest, const ModbusOperationStatus mbOpStatus);

    ModbusOperationStatus getModbusOperationStatus() const;
    std::vector<uint8_t> getReadBitValues() const;
    std::vector<uint16_t> getReadRegisterValues() const;

    void setReadValues(const ModbusReadValues& vals);

    friend bool operator==(const ModbusTcpResponse& mbResA, const ModbusTcpResponse& mbResB);

    std::string convertToLogString() const override;

private:
    const std::shared_ptr<ModbusTcpRequest> m_mbRequest;
    ModbusOperationStatus m_operationStatus;
    ModbusReadValues m_readValues; // 1-bit or 16-bit values

    int computeLengthFieldInByte() const;
    std::vector<uint8_t> getDataBytesInByteVector() const;
};

}
