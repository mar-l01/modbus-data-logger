#pragma once

#include "domain/gateway/includes/ModbusTcpMessageFrame.hpp"

#include <memory>

namespace Gateway {
class ModbusTcpRequest;

class ModbusTcpResponse : public ModbusTcpMessageFrame
{
public:
    ModbusTcpResponse();
    ModbusTcpResponse(const std::vector<uint8_t>& mbTcpRes, const std::shared_ptr<ModbusTcpRequest>& mbTcpReq);

    uint8_t getNumberOfBytesOfReadValues() const;
    std::vector<uint8_t> getReadBitValues() const;
    std::vector<uint16_t> getReadRegisterValues() const;

private:
    std::shared_ptr<ModbusTcpRequest> m_accordingModbusTcpRequest;
};

}
