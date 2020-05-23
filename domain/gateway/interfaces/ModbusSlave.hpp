#pragma once

#include "domain/entity/includes/ModbusDataMapping.hpp"
#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace Gateway {

class ModbusSlave
{
public:
    virtual void setModbusDataMapping(const Entity::ModbusDataMapping& mbMapping) = 0;
    virtual void bind(const std::string& ipAddr, const int port) = 0;
    virtual int listen(const int nbConns) = 0;
    virtual void accept(int& socket) = 0;
    virtual int receive(std::shared_ptr<Entity::ModbusTcpRequest>& request) = 0;
    virtual int reply(std::shared_ptr<Entity::ModbusTcpResponse>& response) = 0;
    virtual void close() = 0;
};

}
