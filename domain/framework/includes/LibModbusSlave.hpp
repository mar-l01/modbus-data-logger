#pragma once

#include "domain/gateway/interfaces/ModbusSlave.hpp"
#include "modbus/modbus.h"

#include <functional>
#include <memory>
#include <string>

namespace Framework {

class LibModbusSlave : public Gateway::ModbusSlave
{
public:
    LibModbusSlave();

    void setupModbusContext(const std::string& ipAddr, const int port) override;
    void setupModbusMapping(const Gateway::ModbusDataMapping& mbMapping) override;
    void listenAndAcceptIncomingConnection() override;
    void processIncomingRequests() override;
    void closeConnection() override;

private:
    std::unique_ptr<modbus_t, std::function<void(modbus_t*)>> m_modbusContext;
    std::unique_ptr<modbus_mapping_t, std::function<void(modbus_mapping_t*)>> m_modbusMapping;
    int m_slaveSocket;
};

}
