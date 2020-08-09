#pragma once

#include "domain/gateway/interfaces/ModbusSlave.hpp"

#include "modbus/modbus.h"

#include <functional>
#include <memory>

namespace Framework {

/**
 * @brief Constants which are internally used by the LibModbus slave implementation
 */
namespace LibModbusConstants {
/**
 * @brief errno value, which indicates a closed connection by the Modbus master
 */
constexpr const int CONNECTION_CLOSED_BY_MASTER = 104;
}

/**
 * @brief This class represents a Modbus slave implemented with the 'libmodbus'-framework.
 * It derives from @ref Gateway::ModbusSlave, which internally represents the Modbus slave.
 *
 * For further information about the 'libmodbus'-framwork see: https://github.com/stephane/libmodbus
 */
class LibModbusSlave : public Gateway::ModbusSlave
{
public:
    /**
     * @brief Construct a new LibModbusSlave object.
     */
    LibModbusSlave();

    /**
     * @see ModbusSlave::setModbusDataMapping
     */
    void setModbusDataMapping(const Entity::ModbusDataMapping& mbMapping) override;

    /**
     * @see ModbusSlave::bind
     */
    void bind(const std::string& ipAddr, const int port) override;

    /**
     * @see ModbusSlave::listen
     */
    int listen(const int nbConns) override;

    /**
     * @see ModbusSlave::accept
     */
    void accept(int& socket) override;

    /**
     * @see ModbusSlave::receive
     */
    Gateway::ModbusReceiveStatus receive(std::shared_ptr<Entity::ModbusTcpRequest>& request) override;

    /**
     * @see ModbusSlave::reply
     */
    Gateway::ModbusReceiveStatus reply(std::shared_ptr<Entity::ModbusTcpResponse>& response) override;

    /**
     * @see ModbusSlave::replyException
     */
    Gateway::ModbusReceiveStatus replyException(Entity::ModbusExceptionCode mbExceptionCode) override;

    /**
     * @see ModbusSlave::close
     */
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
