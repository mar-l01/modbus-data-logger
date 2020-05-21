#pragma once

#include "domain/gateway/includes/ModbusDataMapping.hpp"
#include "domain/gateway/includes/ModbusTcpMessageFrame.hpp"

#include <cstdint>
#include <vector>

namespace Gateway {

class ModbusSlave
{
public:
    virtual void setModbusDataMapping(const ModbusDataMapping& mbMapping) = 0;
    virtual void bind(const std::string& ipAddr, const int port) = 0;
    virtual int listen(const int nbConns) = 0;
    virtual void accept(int& socket) = 0;
    virtual int receive(ModbusTcpMessageFrame& request) = 0;
    virtual int reply(ModbusTcpMessageFrame& response) = 0;
    virtual void close() = 0;
};

}
