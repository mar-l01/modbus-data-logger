#pragma once

#include "domain/entity/includes/ModbusTcpMessageFrame.hpp"
#include "domain/entity/interfaces/ModbusDataLog.hpp"

#include <memory>

namespace Entity {

class ModbusTcpRequest
    : public ModbusTcpMessageFrame
    , public ModbusDataLog
{
public:
    ModbusTcpRequest();
    ModbusTcpRequest(const std::vector<uint8_t>& mbTcpReq);

    uint8_t getNumberOfBytesOfValuesToWrite() const;
    std::vector<uint8_t> getCoilValuesToWrite() const;
    std::vector<uint16_t> getHoldingRegisterValuesToWrite() const;

    friend bool operator==(const ModbusTcpRequest& mbReqA, const ModbusTcpRequest& mbReqB);

    std::string convertToLogString() const override;
};

}
