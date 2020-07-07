#include "domain/framework/includes/LibModbusSlave.hpp"

#include "domain/entity/includes/ModbusTcpConstants.hpp"

#include "spdlog/fmt/ostr.h" // required to let spdlog know about overloaded << operator
#include "spdlog/spdlog.h"
#include <errno.h>

namespace Framework {

LibModbusSlave::LibModbusSlave()
    : m_messageLength(-1)
{}

void LibModbusSlave::setModbusDataMapping(const Entity::ModbusDataMapping& mbMapping)
{
    // clang-format off
    m_modbusMapping = std::move(std::unique_ptr<modbus_mapping_t, std::function<void(modbus_mapping_t*)>>(modbus_mapping_new_start_address(
        mbMapping.startAddressCoils, mbMapping.nbCoils,
        mbMapping.startAddressDiscreteInputs, mbMapping.nbDiscreteInputs,
        mbMapping.startAddressHoldingRegisters, mbMapping.nbHoldingRegisters,
        mbMapping.startAddressInputRegisters, mbMapping.nbInputRegisters),
        [this](modbus_mapping_t* mbMapping){
            // use custom-deleter provided by libmodbus
            modbus_mapping_free(mbMapping);
        }));
    // clang-format on

    // error handling
    if (m_modbusMapping == nullptr) {
        SPDLOG_ERROR("Failed to allocate the mapping");
        SPDLOG_ERROR("- Data-Mapping: {0}", mbMapping);
    }
}

void LibModbusSlave::bind(const std::string& ipAddr, const int port)
{
    // clang-format off
    m_modbusContext = std::move(std::unique_ptr<modbus_t, std::function<void(modbus_t*)>>(modbus_new_tcp(ipAddr.c_str(), port),
        [this](modbus_t* mbCtx) {
            // use custom-deleter provided by libmodbus
            modbus_free(mbCtx);
        }));
    // clang-format on

    // error handling
    if (m_modbusContext == nullptr) {
        SPDLOG_ERROR("Unable to allocate libmodbus context");
        SPDLOG_ERROR("- IP: {0}", ipAddr);
        SPDLOG_ERROR("- Port: {0:d}", port);
    }
#ifdef DEBUG
    else {
        modbus_set_debug(m_modbusContext.get(), true);
    }
#endif
}

int LibModbusSlave::listen(const int nbConns)
{
    SPDLOG_DEBUG("Listen for a incoming connection");

    auto slaveSocket = modbus_tcp_listen(m_modbusContext.get(), nbConns);

    return slaveSocket;
}

void LibModbusSlave::accept(int& socket)
{
    modbus_tcp_accept(m_modbusContext.get(), &socket);

    SPDLOG_DEBUG("Accepted incoming connection");
}

Gateway::ModbusReceiveStatus LibModbusSlave::receive(std::shared_ptr<Entity::ModbusTcpRequest>& request)
{
    auto mbRequest = std::vector<uint8_t>(MODBUS_TCP_MAX_ADU_LENGTH);

    m_messageLength = modbus_receive(m_modbusContext.get(), mbRequest.data());

    // save request (required for sending response)
    m_lastRequest = mbRequest;

    // convert byte vector into transferable object
    request = std::make_shared<Entity::ModbusTcpRequest>(mbRequest);

    return getModbusReceiveStatus(m_messageLength);
}

Gateway::ModbusReceiveStatus LibModbusSlave::reply(std::shared_ptr<Entity::ModbusTcpResponse>& response)
{
    // if request wants to read values, update internal mapping accordingly
    updateMappingIfNeeded(response);

    m_messageLength = modbus_reply(m_modbusContext.get(), m_lastRequest.data(), m_messageLength, m_modbusMapping.get());

    return getModbusReceiveStatus(m_messageLength);
}

Gateway::ModbusReceiveStatus LibModbusSlave::replyException(Entity::ModbusExceptionCode mbExceptionCode)
{
    m_messageLength =
      modbus_reply_exception(m_modbusContext.get(), m_lastRequest.data(), mbExceptionCode._to_integral());

    return getModbusReceiveStatus(m_messageLength);
}

void LibModbusSlave::close()
{
    modbus_close(m_modbusContext.get());
}

Gateway::ModbusReceiveStatus LibModbusSlave::getModbusReceiveStatus(int requestLength) const
{
    Gateway::ModbusReceiveStatus mbRecStatus = Gateway::ModbusReceiveStatus::OK;

    if (requestLength == -1) {
        // errno indicates a closed connection by Modbus master
        if (errno == LibModbusConstants::CONNECTION_CLOSED_BY_MASTER) {
            mbRecStatus = Gateway::ModbusReceiveStatus::CONNECTION_CLOSED_BY_MASTER;
        } else {
            mbRecStatus = Gateway::ModbusReceiveStatus::FAILED;
        }
    } else if (requestLength == 0) {
        mbRecStatus = Gateway::ModbusReceiveStatus::IGNORED;
    } // else everything is fine (> 0)

    return mbRecStatus;
}

void LibModbusSlave::updateMappingIfNeeded(const std::shared_ptr<Entity::ModbusTcpResponse>& response)
{
    // extract function code
    auto fc = getFunctionCode();

    // update mapping if a read operation took place (0x01, 0x02, 0x03, 0x04)
    switch (fc) {
        case static_cast<uint8_t>(Entity::ModbusFunctionCode::READ_COIL_VALUES):
            updateCoilValues(response->getReadBitValues());
            break;
        case static_cast<uint8_t>(Entity::ModbusFunctionCode::READ_DISCRETE_INPUT_VALUES):
            updateDiscreteInputValues(response->getReadBitValues());
            break;
        case static_cast<uint8_t>(Entity::ModbusFunctionCode::READ_HOLDING_REGISTER_VALUES):
            updateHoldingRegisterValues(response->getReadRegisterValues());
            break;
        case static_cast<uint8_t>(Entity::ModbusFunctionCode::READ_INPUT_REGISTER_VALUES):
            updateInputRegisterValues(response->getReadRegisterValues());
            break;
        default:
            SPDLOG_WARN("No read operation took place");
            break;
    }
}

uint8_t LibModbusSlave::getFunctionCode() const
{
    return m_lastRequest[Entity::ModbusMessageFrameByte::FUNCTION_CODE];
}

uint16_t LibModbusSlave::getStartAddress() const
{
    auto startAddr = static_cast<uint16_t>(m_lastRequest[Entity::ModbusMessageFrameByte::DATA_BYTES] << 8) +
                     m_lastRequest[Entity::ModbusMessageFrameByte::DATA_BYTES + 1];

    return startAddr;
}

void LibModbusSlave::updateCoilValues(const std::vector<uint8_t>& values)
{
    auto startAddr = getStartAddress();

    for (int i = 0; i < m_modbusMapping->nb_bits; ++i) {
        m_modbusMapping->tab_bits[startAddr - m_modbusMapping->start_bits + i] = values[i];
    }
}

void LibModbusSlave::updateDiscreteInputValues(const std::vector<uint8_t>& values)
{
    auto startAddr = getStartAddress();

    for (int i = 0; i < m_modbusMapping->nb_input_bits; ++i) {
        m_modbusMapping->tab_input_bits[startAddr - m_modbusMapping->start_input_bits + i] = values[i];
    }
}

void LibModbusSlave::updateHoldingRegisterValues(const std::vector<uint16_t>& values)
{
    auto startAddr = getStartAddress();

    for (int i = 0; i < m_modbusMapping->nb_registers; ++i) {
        m_modbusMapping->tab_registers[startAddr - m_modbusMapping->start_registers + i] = values[i];
    }
}

void LibModbusSlave::updateInputRegisterValues(const std::vector<uint16_t>& values)
{
    auto startAddr = getStartAddress();

    for (int i = 0; i < m_modbusMapping->nb_input_registers; ++i) {
        m_modbusMapping->tab_input_registers[startAddr - m_modbusMapping->start_input_registers + i] = values[i];
    }
}

}
