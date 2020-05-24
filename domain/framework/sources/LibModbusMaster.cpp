#include "domain/framework/includes/LibModbusMaster.hpp"

namespace Framework {

LibModbusMaster::LibModbusMaster() {}

void LibModbusMaster::connect(const std::string& ipAddr, const int port)
{
    // clang-format off
    m_modbusContext = std::move(std::unique_ptr<modbus_t, std::function<void(modbus_t*)>>(modbus_new_tcp(ipAddr.c_str(), port),
            [this](modbus_t* mbCtx) {
                // use custom-deleter provided by libmodbus
                modbus_free(mbCtx);
            }));
    // clang-format on

#ifdef DEBUG
    // error handling
    if (m_modbusContext == nullptr) {
        std::cerr << "[LibModbusMaster] Unable to allocate libmodbus context\n";
        std::cerr << "[LibModbusMaster] - IP: " << ipAddr << '\n';
        std::cerr << "[LibModbusMaster] - Port: " << port << '\n';
    } else {
        modbus_set_debug(m_modbusContext.get(), true);
    }
#endif

    // connect
    auto connSuccessful = modbus_connect(m_modbusContext.get());

#ifdef DEBUG
    // error handling
    if (connSuccessful == -1) {
        std::cerr << "[LibModbusMaster] Failed to connect to slave\n";
    }
#endif
}

Gateway::ModbusReadOperationResult<uint8_t> LibModbusMaster::readCoilValues(int startAddress, int nbValues)
{
    return readValues<uint8_t>(modbus_read_bits, startAddress, nbValues);
}

Gateway::ModbusReadOperationResult<uint8_t> LibModbusMaster::readDiscreteInputValues(int startAddress, int nbValues)
{
    return readValues<uint8_t>(modbus_read_input_bits, startAddress, nbValues);
}

Gateway::ModbusReadOperationResult<uint16_t> LibModbusMaster::readHoldingRegisterValues(int startAddress, int nbValues)
{
    return readValues<uint16_t>(modbus_read_registers, startAddress, nbValues);
}

Gateway::ModbusReadOperationResult<uint16_t> LibModbusMaster::readInputRegisterValues(int startAddress, int nbValues)
{
    return readValues<uint16_t>(modbus_read_input_registers, startAddress, nbValues);
}

Gateway::ModbusOperationStatus LibModbusMaster::writeSingleCoilValue(int startAddress, uint8_t coilValue)
{
    return writeSingleValue<int>(modbus_write_bit, startAddress, static_cast<int>(coilValue));
}

Gateway::ModbusOperationStatus LibModbusMaster::writeSingleHoldingRegisterValue(int startAddress,
                                                                                uint16_t registerValue)
{
    return writeSingleValue<uint16_t>(modbus_write_register, startAddress, registerValue);
}

Gateway::ModbusOperationStatus LibModbusMaster::writeMultipleCoilValues(int startAddress,
                                                                        const std::vector<uint8_t>& coilValues)
{
    return writeValues<uint8_t>(modbus_write_bits, startAddress, coilValues);
}

Gateway::ModbusOperationStatus LibModbusMaster::writeMultipleHoldingRegisterValues(
  int startAddress, const std::vector<uint16_t>& registerValues)
{
    return writeValues<uint16_t>(modbus_write_registers, startAddress, registerValues);
}

void LibModbusMaster::close()
{
    modbus_close(m_modbusContext.get());
}

template<typename T>
Gateway::ModbusReadOperationResult<T> LibModbusMaster::readValues(int (*libmodbusReadFunction)(modbus_t*, int, int, T*),
                                                                  int sAddr, int nbVals)
{
    // allocate enough memory for values to be read
    std::vector<T> readValuesVector(nbVals);

    // execute libmodbus function with respective parameters
    auto rc = libmodbusReadFunction(m_modbusContext.get(), sAddr, nbVals, readValuesVector.data());

    // set operation status depending on return code of function above
    auto operationStatus = (rc == -1) ? Gateway::ModbusOperationStatus::FAIL : Gateway::ModbusOperationStatus::SUCCESS;

    return Gateway::ModbusReadOperationResult<T>(operationStatus, readValuesVector);
}

template<typename T>
Gateway::ModbusOperationStatus LibModbusMaster::writeSingleValue(int (*libmodbusSingleWriteFunction)(modbus_t*, int, T),
                                                                 int sAddr, uint16_t value)
{
    // execute libmodbus function with respective parameters
    auto rc = libmodbusSingleWriteFunction(m_modbusContext.get(), sAddr, value);

    // set operation status depending on return code of function above
    return (rc == -1) ? Gateway::ModbusOperationStatus::FAIL : Gateway::ModbusOperationStatus::SUCCESS;
}

template<typename T>
Gateway::ModbusOperationStatus LibModbusMaster::writeValues(int (*libmodbusWriteFunction)(modbus_t*, int, int,
                                                                                          const T*),
                                                            int sAddr, std::vector<T> values)
{
    // execute libmodbus function with respective parameters (number of values to write is required := size of vector)
    auto rc = libmodbusWriteFunction(m_modbusContext.get(), sAddr, values.size(), values.data());

    // set operation status depending on return code of function above
    return (rc == -1) ? Gateway::ModbusOperationStatus::FAIL : Gateway::ModbusOperationStatus::SUCCESS;
}

}
