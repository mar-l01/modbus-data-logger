#pragma once

namespace Framework {

/**
 * @brief A list of different frameworks which can be used for logging
 */
enum class LoggingFramework
{
    SPDLOG,
    OTHER_LOGGING_FRAMEWORK
};

/**
 * @brief A list of different frameworks to read different file formats.
 */
enum class FileReaderFramework
{
    NLOHMANN_JSON,
    OTHER_READER_FRAMEWORK
};

}

namespace Gateway {

/**
 * @brief A list of different frameworks to be used to create a Modbus slave or master
 */
enum class ModbusComponentsFramework
{
    LIBMODBUS,
    OTHER_MODBUS_FRAMEWORK
};

}
