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

// currently available Modbus function codes
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

}
