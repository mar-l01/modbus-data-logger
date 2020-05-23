#pragma once

#include "domain/gateway/includes/ModbusDataMapping.hpp"
#include "domain/gateway/includes/ModbusTcpRequest.hpp"
#include "domain/gateway/includes/ModbusTcpResponse.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace Gateway {

class ModbusSlave
{
public:
    virtual void setModbusDataMapping(const ModbusDataMapping& mbMapping) = 0;
    virtual void bind(const std::string& ipAddr, const int port) = 0;
    virtual int listen(const int nbConns) = 0;
    virtual void accept(int& socket) = 0;
    virtual int receive(std::shared_ptr<ModbusTcpRequest>& request) = 0;
    virtual int reply(std::shared_ptr<ModbusTcpResponse>& response) = 0;
    virtual void close() = 0;
};

}
