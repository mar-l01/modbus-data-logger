#pragma once

#include "domain/gateway/datatypes/includes/ModbusDataMapping.hpp"

namespace Gateway {
class ModbusSlave
{
    virtual void setupModbusContext(const std::string& ipAddr, const int port) = 0;
    virtual void setupModbusMapping(const Gateway::ModbusDataMapping& mbMapping) = 0;
    virtual void listenAndAcceptIncomingConnection() = 0;
    virtual void processIncomingRequests() = 0;
    virtual void closeConnection() = 0;
};
}
