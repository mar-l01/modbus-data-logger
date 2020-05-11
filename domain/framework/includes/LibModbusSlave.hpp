#pragma once

#include "domain/gateway/interfaces/ModbusSlave.hpp"
#include "modbus/modbus.h"

#include <functional>
#include <memory>
#include <string>

// use debug messages for libmodbus library
#define DEBUG

namespace Framework {

class LibModbusSlave : public Gateway::ModbusSlave
{
public:
    LibModbusSlave();

    bool init(const std::string& ipAddr, const int port, const Gateway::ModbusDataMapping& mbMapping) override;
    void run() override;

private:
    std::unique_ptr<modbus_t, std::function<void(modbus_t*)>> m_modbusContext;
    std::unique_ptr<modbus_mapping_t, std::function<void(modbus_mapping_t*)>> m_modbusMapping;
    int m_slaveSocket;

    void initModbusContext(const std::string& ipAddr, const int port);
    void initModbusMapping(const Gateway::ModbusDataMapping& mbMapping);
};

}
