#pragma once

#include "domain/entity/includes/ModbusDataMapping.hpp"

namespace FixtureTestConstants {

constexpr const char* MODBUS_IP_ADDRESS = "127.0.0.1";
constexpr const int MODBUS_PORT = 5002;

constexpr const int MODBUS_START_ADDRESS_COILS = 0;
constexpr const int MODBUS_START_ADDRESS_DISCRETE_INPUTS = 0;
constexpr const int MODBUS_START_ADDRESS_HOLDING_REGISTERS = 0;
constexpr const int MODBUS_START_ADDRESS_INPUT_REGISTERS = 0;
constexpr const int MODBUS_NUMBER_COILS = 10;
constexpr const int MODBUS_NUMBER_DISCRETE_INPUTS = 10;
constexpr const int MODBUS_NUMBER_HOLDING_REGISTERS = 10;
constexpr const int MODBUS_NUMBER_INPUT_REGISTERS = 10;

constexpr Entity::ModbusDataMapping MODBUS_DATA_MAPPING = {MODBUS_START_ADDRESS_COILS,
                                                           MODBUS_START_ADDRESS_DISCRETE_INPUTS,
                                                           MODBUS_START_ADDRESS_HOLDING_REGISTERS,
                                                           MODBUS_START_ADDRESS_INPUT_REGISTERS,
                                                           MODBUS_NUMBER_COILS,
                                                           MODBUS_NUMBER_DISCRETE_INPUTS,
                                                           MODBUS_NUMBER_HOLDING_REGISTERS,
                                                           MODBUS_NUMBER_INPUT_REGISTERS};

}
