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

    void setModbusDataMapping(const Gateway::ModbusDataMapping& mbMapping) override;
    void bind(const std::string& ipAddr, const int port) override;
    int listen(const int nbConns) override;
    void accept(int& socket) override;
    int receive(Gateway::ModbusTcpMessageFrame& request) override;
    int reply(Gateway::ModbusTcpMessageFrame& response) override;
    void close() override;

private:
    std::unique_ptr<modbus_t, std::function<void(modbus_t*)>> m_modbusContext;
    std::unique_ptr<modbus_mapping_t, std::function<void(modbus_mapping_t*)>> m_modbusMapping;
    int m_messageLength;
    std::vector<uint8_t> m_lastRequest;
};

}
