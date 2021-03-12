#include "domain/framework/includes/JsonFileAccessor.hpp"

#include "spdlog/spdlog.h"
#include <fstream>
#include <nlohmann/json.hpp>

namespace Entity {

void to_json(nlohmann::json& j, const ModbusConfiguration& mbConfig) // NOLINT(readability-identifier-naming)
{
    j = nlohmann::json{
      {"internal_modbus_slave", {"ip_address", mbConfig.ipAddrIntSlave}},
      {"internal_modbus_slave", {"port", mbConfig.portIntSlave}},

      {"external_modbus_slave", {"ip_address", mbConfig.ipAddrExtSlave}},
      {"external_modbus_slave", {"port", mbConfig.portExtSlave}},

      {"modbus_data_mapping", {"coil", {"start_address", mbConfig.dataMapping.startAddressCoils}}},
      {"modbus_data_mapping", {"coil", {"number_of_values", mbConfig.dataMapping.nbCoils}}},
      {"modbus_data_mapping", {"discrete_input", {"start_address", mbConfig.dataMapping.startAddressDiscreteInputs}}},
      {"modbus_data_mapping", {"discrete_input", {"number_of_values", mbConfig.dataMapping.nbDiscreteInputs}}},
      {"modbus_data_mapping",
       {"holding_register", {"start_address", mbConfig.dataMapping.startAddressHoldingRegisters}}},
      {"modbus_data_mapping", {"holding_register", {"number_of_values", mbConfig.dataMapping.nbHoldingRegisters}}},
      {"modbus_data_mapping", {"input_register", {"start_address", mbConfig.dataMapping.startAddressInputRegisters}}},
      {"modbus_data_mapping", {"input_register", {"number_of_values", mbConfig.dataMapping.nbInputRegisters}}},

      {"modbus_timeout_in_ms", mbConfig.modbusTimeout},
      {"application_timeout_in_ms", mbConfig.applicationTimeout}};
}

void from_json(const nlohmann::json& j, ModbusConfiguration& mbConfig) // NOLINT(readability-identifier-naming)
{
    j.at("internal_modbus_slave").at("ip_address").get_to(mbConfig.ipAddrIntSlave);
    j.at("internal_modbus_slave").at("port").get_to(mbConfig.portIntSlave);

    j.at("external_modbus_slave").at("ip_address").get_to(mbConfig.ipAddrExtSlave);
    j.at("external_modbus_slave").at("port").get_to(mbConfig.portExtSlave);

    j.at("modbus_data_mapping").at("coil").at("start_address").get_to(mbConfig.dataMapping.startAddressCoils);
    j.at("modbus_data_mapping").at("coil").at("number_of_values").get_to(mbConfig.dataMapping.nbCoils);
    j.at("modbus_data_mapping")
      .at("discrete_input")
      .at("start_address")
      .get_to(mbConfig.dataMapping.startAddressDiscreteInputs);
    j.at("modbus_data_mapping")
      .at("discrete_input")
      .at("number_of_values")
      .get_to(mbConfig.dataMapping.nbDiscreteInputs);
    j.at("modbus_data_mapping")
      .at("holding_register")
      .at("start_address")
      .get_to(mbConfig.dataMapping.startAddressHoldingRegisters);
    j.at("modbus_data_mapping")
      .at("holding_register")
      .at("number_of_values")
      .get_to(mbConfig.dataMapping.nbHoldingRegisters);
    j.at("modbus_data_mapping")
      .at("input_register")
      .at("start_address")
      .get_to(mbConfig.dataMapping.startAddressInputRegisters);
    j.at("modbus_data_mapping")
      .at("input_register")
      .at("number_of_values")
      .get_to(mbConfig.dataMapping.nbInputRegisters);

    j.at("modbus_timeout_in_ms").get_to(mbConfig.modbusTimeout);
    j.at("application_timeout_in_ms").get_to(mbConfig.applicationTimeout);
}

}

namespace Framework {

JsonFileAccessor::JsonFileAccessor()
    : m_modbusConfiguration(Entity::ModbusConfiguration())
{}

void JsonFileAccessor::readConfigurationFile(const std::string& path)
{
    std::ifstream jsonFs(path);
    if (!jsonFs) {
        SPDLOG_ERROR("Failed to open file: {0}", path);
        return;
    }

    try {
        nlohmann::json j;
        jsonFs >> j;

        m_modbusConfiguration = j.get<Entity::ModbusConfiguration>();
    } catch (std::exception& ex) {
        SPDLOG_ERROR("Error: {0}.", ex.what());
    }
}

Entity::ModbusConfiguration JsonFileAccessor::getModbusConfiguration() const
{
    return m_modbusConfiguration;
}

}
