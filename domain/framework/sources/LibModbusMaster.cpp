#include "domain/framework/includes/LibModbusMaster.hpp"

#include "spdlog/spdlog.h"

namespace Framework {

LibModbusMaster::LibModbusMaster()
    : m_isConnected(-1)
{}

void LibModbusMaster::connect(const std::string& ipAddr, const int port)
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

    // connect
    m_isConnected = modbus_connect(m_modbusContext.get());

    // error handling
    if (m_isConnected == -1) {
        SPDLOG_ERROR("Failed to connect to slave");
    }
}

void LibModbusMaster::reconnect()
{
    // connect
    m_isConnected = modbus_connect(m_modbusContext.get());

    // error handling
    if (m_isConnected == -1) {
        SPDLOG_ERROR("Failed to re-connect to slave");
    }
}

void LibModbusMaster::setResponseTimeout(const uint16_t timeoutInMs)
{
    // set timeout applied when waiting for a response of the Modbus slave
    // if timeout occurs, ETIMEDOUT is set for calling function
    modbus_set_response_timeout(m_modbusContext.get(), 0, timeoutInMs * 1000);
}

Entity::ModbusReadOperationResult<uint8_t> LibModbusMaster::readCoilValues(int startAddress, int nbValues)
{
    return readValues<uint8_t>(modbus_read_bits, startAddress, nbValues);
}

Entity::ModbusReadOperationResult<uint8_t> LibModbusMaster::readDiscreteInputValues(int startAddress, int nbValues)
{
    return readValues<uint8_t>(modbus_read_input_bits, startAddress, nbValues);
}

Entity::ModbusReadOperationResult<uint16_t> LibModbusMaster::readHoldingRegisterValues(int startAddress, int nbValues)
{
    return readValues<uint16_t>(modbus_read_registers, startAddress, nbValues);
}

Entity::ModbusReadOperationResult<uint16_t> LibModbusMaster::readInputRegisterValues(int startAddress, int nbValues)
{
    return readValues<uint16_t>(modbus_read_input_registers, startAddress, nbValues);
}

Entity::ModbusOperationStatus LibModbusMaster::writeSingleCoilValue(int startAddress, uint8_t coilValue)
{
    return writeSingleValue<int>(modbus_write_bit, startAddress, static_cast<int>(coilValue));
}

Entity::ModbusOperationStatus LibModbusMaster::writeSingleHoldingRegisterValue(int startAddress, uint16_t registerValue)
{
    return writeSingleValue<int>(modbus_write_register, startAddress, registerValue);
}

Entity::ModbusOperationStatus LibModbusMaster::writeMultipleCoilValues(int startAddress,
                                                                       const std::vector<uint8_t>& coilValues)
{
    return writeValues<uint8_t>(modbus_write_bits, startAddress, coilValues);
}

Entity::ModbusOperationStatus LibModbusMaster::writeMultipleHoldingRegisterValues(
  int startAddress, const std::vector<uint16_t>& registerValues)
{
    return writeValues<uint16_t>(modbus_write_registers, startAddress, registerValues);
}

void LibModbusMaster::close()
{
    modbus_close(m_modbusContext.get());
    m_isConnected = -1; // assume connection was closed
}

bool LibModbusMaster::isConnected()
{
    return (m_isConnected == 0);
}

template<typename T>
Entity::ModbusReadOperationResult<T> LibModbusMaster::readValues(int (*libmodbusReadFunction)(modbus_t*, int, int, T*),
                                                                 int sAddr, int nbVals)
{
    // allocate enough memory for values to be read
    std::vector<T> readValuesVector(nbVals);

    // execute libmodbus function with respective parameters
    auto rc = libmodbusReadFunction(m_modbusContext.get(), sAddr, nbVals, readValuesVector.data());

    // set operation status depending on return code of function above
    auto operationStatus = mapReturnCodeToOperationStatus(rc);

    return Entity::ModbusReadOperationResult<T>(operationStatus, readValuesVector);
}

template<typename T>
Entity::ModbusOperationStatus LibModbusMaster::writeSingleValue(int (*libmodbusSingleWriteFunction)(modbus_t*, int, T),
                                                                int sAddr, uint16_t value)
{
    // execute libmodbus function with respective parameters
    auto rc = libmodbusSingleWriteFunction(m_modbusContext.get(), sAddr, value);

    // set operation status depending on return code of function above
    return mapReturnCodeToOperationStatus(rc);
}

template<typename T>
Entity::ModbusOperationStatus LibModbusMaster::writeValues(int (*libmodbusWriteFunction)(modbus_t*, int, int, const T*),
                                                           int sAddr, std::vector<T> values)
{
    // execute libmodbus function with respective parameters (number of values to write is required := size of vector)
    auto rc = libmodbusWriteFunction(m_modbusContext.get(), sAddr, values.size(), values.data());

    // set operation status depending on return code of function above
    return mapReturnCodeToOperationStatus(rc);
}

Entity::ModbusOperationStatus LibModbusMaster::mapReturnCodeToOperationStatus(const int returnCode)
{
    Entity::ModbusOperationStatus mbOpStatus = Entity::ModbusOperationStatus::SUCCESS;

    // -1 is returned in error case / errno gets set
    if (returnCode == -1) {
        if (errno == ETIMEDOUT) {
            mbOpStatus = Entity::ModbusOperationStatus::TIMEOUT;
            SPDLOG_ERROR("Connection timed out..");
        } else {
            mbOpStatus = Entity::ModbusOperationStatus::FAIL;
        }
    }

    return mbOpStatus;
}

}
