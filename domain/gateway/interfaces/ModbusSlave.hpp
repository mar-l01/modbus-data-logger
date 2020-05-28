#pragma once

#include "domain/entity/includes/ModbusDataMapping.hpp"
#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace Gateway {

// used to indicate success/errors when receiving a Modbus request
enum class ModbusReceiveStatus
{
    IGNORED,                    // incoming request was ignored (some special case)
    OK,                         // successfully received request
    FAILED,                     // failed to receive request
    CONNECTION_CLOSED_BY_MASTER // Modbus master closed connection
};

class ModbusSlave
{
public:
    virtual void setModbusDataMapping(const Entity::ModbusDataMapping& mbMapping) = 0;
    virtual void bind(const std::string& ipAddr, const int port) = 0;
    virtual int listen(const int nbConns) = 0;
    virtual void accept(int& socket) = 0;
    virtual Gateway::ModbusReceiveStatus receive(std::shared_ptr<Entity::ModbusTcpRequest>& request) = 0;
    virtual Gateway::ModbusReceiveStatus reply(std::shared_ptr<Entity::ModbusTcpResponse>& response) = 0;
    virtual void close() = 0;
};

}
