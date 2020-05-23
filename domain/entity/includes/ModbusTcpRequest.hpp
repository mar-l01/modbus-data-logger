#pragma once

#include "domain/entity/includes/ModbusTcpMessageFrame.hpp"

#include <memory>

namespace Entity {

class ModbusTcpRequest : public ModbusTcpMessageFrame
{
public:
    ModbusTcpRequest();
    ModbusTcpRequest(const std::vector<uint8_t>& mbTcpReq);

    uint8_t getNumberOfBytesOfValuesToWrite() const;
    std::vector<uint8_t> getCoilValuesToWrite() const;
    std::vector<uint16_t> getHoldingRegisterValuesToWrite() const;
};

}
