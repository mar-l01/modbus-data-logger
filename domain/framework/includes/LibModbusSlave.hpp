#pragma once

#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include "modbus/modbus.h"

#include <functional>
#include <memory>


// use debug messages for libmodbus library
#define DEBUG

namespace Framework {

namespace LibModbusConstants {
constexpr const int CONNECTION_CLOSED_BY_MASTER = 104;
}

class LibModbusSlave : public Gateway::ModbusSlave
{
public:
    LibModbusSlave();

    void setModbusDataMapping(const Entity::ModbusDataMapping& mbMapping) override;
    void bind(const std::string& ipAddr, const int port) override;
    int listen(const int nbConns) override;
    void accept(int& socket) override;
    Gateway::ModbusReceiveStatus receive(std::shared_ptr<Entity::ModbusTcpRequest>& request) override;
    Gateway::ModbusReceiveStatus reply(std::shared_ptr<Entity::ModbusTcpResponse>& response) override;
    void close() override;

private:
    std::unique_ptr<modbus_t, std::function<void(modbus_t*)>> m_modbusContext;
    std::unique_ptr<modbus_mapping_t, std::function<void(modbus_mapping_t*)>> m_modbusMapping;
    int m_messageLength;
    std::vector<uint8_t> m_lastRequest;

    Gateway::ModbusReceiveStatus getModbusReceiveStatus(int requestLength) const;
    void updateMappingIfNeeded(const std::shared_ptr<Entity::ModbusTcpResponse>& response);
    uint8_t getFunctionCode() const;
    uint16_t getStartAddress() const;
    void updateCoilValues(const std::vector<uint8_t>& values);
    void updateDiscreteInputValues(const std::vector<uint8_t>& values);
    void updateHoldingRegisterValues(const std::vector<uint16_t>& values);
    void updateInputRegisterValues(const std::vector<uint16_t>& values);
};

}
