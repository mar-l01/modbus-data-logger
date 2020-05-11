#pragma once

#include "domain/gateway/datatypes/includes/ModbusDataMapping.hpp"

namespace Gateway {

class ModbusSlave
{
public:
    virtual bool init(const std::string& ipAddr, const int port, const Gateway::ModbusDataMapping& mbMapping) = 0;
    virtual void run() = 0;
};

}
