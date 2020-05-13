#pragma once

#include "domain/gateway/includes/ModbusDataMapping.hpp"

#include <cstdint>
#include <vector>

namespace Gateway {

class ModbusSlave
{
public:
    virtual void setModbusDataMapping(const Gateway::ModbusDataMapping& mbMapping) = 0;
    virtual void bind(const std::string& ipAddr, const int port) = 0;
    virtual int listen(const int nbConns) = 0;
    virtual void accept(int& socket) = 0;
    virtual int receive(std::vector<uint8_t>& request) = 0;
    virtual int reply(std::vector<uint8_t>& response) = 0;
    virtual void close() = 0;
};

}
