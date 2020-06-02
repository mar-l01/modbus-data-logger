#pragma once

#include <enum.h>

namespace Entity {

// bytes of respective field inside a Modbus message frame
namespace ModbusMessageFrameByte {
constexpr const int TRANSACTION_ID = 0;
constexpr const int PROTOCOL_ID = 2;
constexpr const int LENGTH_FIELD = 4;
constexpr const int UNIT_ID = 6;
constexpr const int FUNCTION_CODE = 7;
constexpr const int DATA_BYTES = 8;
}

// offsets of respective field inside the Modbus message frames data-bytes
namespace ModbusDataByteOffset {
constexpr const int START_ADDRESS = 0;
constexpr const int READ_VALUES = 1;
constexpr const int NUMBER_OF_VALUES_TO_READ_OR_WRITE = 2;
constexpr const int SINGLE_VALUE_TO_WRITE = 2;
constexpr const int WRITE_VALUES = 5;
}

// clang-format off
BETTER_ENUM(ModbusFunctionCode, unsigned char,
    // read FCs
    READ_COIL_VALUES = 0x01,
    READ_DISCRETE_INPUT_VALUES,
    READ_HOLDING_REGISTER_VALUES,
    READ_INPUT_REGISTER_VALUES,

    // write FCs
    WRITE_SINGLE_COIL_VALUE,
    WRITE_SINGLE_HOLDING_REGISTER_VALUE,
    WRITE_MULTIPLE_COIL_VALUES = 0x0f,
    WRITE_MULTIPLE_HOLDING_REGISTER_VALUES
)
// clang-format on

// clang-format off
BETTER_ENUM(ModbusExceptionCode, unsigned char,
    ILLEGAL_FUNCTION  = 0x01,
    ILLEGAL_DATA_ADDRESS,
    ILLEGAL_DATA_VALUE,
    SLAVE_DEVICE_FAILURE,
    ACKNOWLEDGE,
    SLAVE_DEVICE_BUSY,
    NEGATIVE_ACKNOWLEDGE,
    MEMORY_PARITY_ERROR,
    // 9 is not defined
    GATEWAY_PATH_UNAVAILABLE = 0x0a,
    GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND
)
// clang-format on

}
