#pragma once

#include "domain/gateway/interfaces/ModbusMaster.hpp"

#include "modbus/modbus.h"

#include <functional>
#include <iostream>
#include <memory>

namespace Framework {

/**
 * @brief This class represents a Modbus master implemented with the 'libmodbus'-framework.
 * It derives from @ref Gateway::ModbusMaster, which internally represents the Modbus master.
 *
 * For further information about the 'libmodbus'-framwork see: https://github.com/stephane/libmodbus
 */
class LibModbusMaster : public Gateway::ModbusMaster
{
public:
    /**
     * @brief Construct a new LibModbusMaster object.
     */
    LibModbusMaster();

    /**
     * @see ModbusMaster::connect
     */
    void connect(const std::string& ipAddr, const int port) override;

    /**
     * @see ModbusMaster::reconnect
     */
    void reconnect() override;

    /**
     * @see ModbusMaster::setResponseTimeout
     */
    void setResponseTimeout(const uint16_t timeoutInMs) override;

    // --- read operations ---
    /**
     * @see ModbusMaster::readCoilValues
     */
    Entity::ModbusReadOperationResult<uint8_t> readCoilValues(int startAddress, int nbValues) override;

    /**
     * @see ModbusMaster::readDiscreteInputValues
     */
    Entity::ModbusReadOperationResult<uint8_t> readDiscreteInputValues(int startAddress, int nbValues) override;

    /**
     * @see ModbusMaster::readHoldingRegisterValues
     */
    Entity::ModbusReadOperationResult<uint16_t> readHoldingRegisterValues(int startAddress, int nbValues) override;

    /**
     * @see ModbusMaster::readInputRegisterValues
     */
    Entity::ModbusReadOperationResult<uint16_t> readInputRegisterValues(int startAddress, int nbValues) override;

    // --- write operations ---
    /**
     * @see ModbusMaster::writeSingleCoilValue
     */
    Entity::ModbusOperationStatus writeSingleCoilValue(int startAddress, uint8_t coilValue) override;

    /**
     * @see ModbusMaster::writeSingleHoldingRegisterValue
     */
    Entity::ModbusOperationStatus writeSingleHoldingRegisterValue(int startAddress, uint16_t registerValue) override;

    /**
     * @see ModbusMaster::writeMultipleCoilValues
     */
    Entity::ModbusOperationStatus writeMultipleCoilValues(int startAddress,
                                                          const std::vector<uint8_t>& coilValues) override;

    /**
     * @see ModbusMaster::writeMultipleHoldingRegisterValues
     */
    Entity::ModbusOperationStatus writeMultipleHoldingRegisterValues(
      int startAddress, const std::vector<uint16_t>& registerValues) override;

    /**
     * @see ModbusMaster::close
     */
    void close() override;

    /**
     * @see ModbusMaster::isConnected
     */
    bool isConnected() override;

private:
    std::unique_ptr<modbus_t, std::function<void(modbus_t*)>> m_modbusContext;
    int m_isConnected;

    template<typename T>
    Entity::ModbusReadOperationResult<T> readValues(int (*libmodbusReadFunction)(modbus_t*, int, int, T*), int sAddr,
                                                    int nbVals);

    template<typename T>
    Entity::ModbusOperationStatus writeSingleValue(int (*libmodbusSingleWriteFunction)(modbus_t*, int, const T),
                                                   int sAddr, uint16_t value);

    template<typename T>
    Entity::ModbusOperationStatus writeValues(int (*libmodbusWriteFunction)(modbus_t*, int, int, const T*), int sAddr,
                                              std::vector<T> values);

    Entity::ModbusOperationStatus mapReturnCodeToOperationStatus(const int returnCode);
};

}
