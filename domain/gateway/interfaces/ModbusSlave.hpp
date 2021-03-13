#pragma once

#include "domain/entity/includes/ModbusDataMapping.hpp"
#include "domain/entity/includes/ModbusTcpConstants.hpp"
#include "domain/entity/includes/ModbusTcpRequest.hpp"
#include "domain/entity/includes/ModbusTcpResponse.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace Gateway {

/**
 * @brief This enumeration is used to indicate success/errors when receiving a Modbus request
 */
enum class ModbusReceiveStatus
{
    IGNORED,                    // incoming request was ignored (some special case)
    OK,                         // successfully received request
    FAILED,                     // failed to receive request
    CONNECTION_CLOSED_BY_MASTER // Modbus master closed connection
};

/**
 * @brief This interface provides methods which need to be implemented by a class to be used as Modbus slave
 * within this application.
 */
class ModbusSlave
{
public:
    /**
     * @brief Configure the Modbus data mapping of the Modbus slave instance.
     *
     * @param mbMapping A @ref Entity::ModbusDataMapping instance, which holds all necessary information about the
     * Modbus data types.
     */
    virtual void setModbusDataMapping(const Entity::ModbusDataMapping& mbMapping) = 0;

    /**
     * @brief As the Modbus slave is acting like a kind of TCP server, this method is used to bind it to a given
     * IP-address and port number.
     *
     * @param ipAddr IP-address this Modbus slave will later listen to.
     * @param port Port number this Modbus slave will later listen on.
     */
    virtual void bind(const std::string& ipAddr, const int port) = 0;

    /**
     * @brief Let the Modbus slave listen of a given number of incoming connections.
     *
     * @param nbConns Total number of connections the Modbus slave should listen to (currently only 1 is supproted).
     * @return A socket file descriptor.
     */
    virtual int listen(const int nbConns) = 0;

    /**
     * @brief Accept an incoming connection (from a Modbus master) and establishes it.
     *
     * @param socket Socket file descriptor which is used to set up the connection.
     */
    virtual void accept(int& socket) = 0;

    /**
     * @brief Wait for an incoming request from a Modbus master. Blocks until a request was received.
     *
     * @param request The incoming Modbus request is saved in this @ref Entity::ModbusTcpRequest instance.
     * @return A status informing about a successful, failed, ... connection.
     */
    virtual Gateway::ModbusReceiveStatus receive(std::shared_ptr<Entity::ModbusTcpRequest>& request) = 0;

    /**
     * @brief Send provided Modbus Tcp response back to the Modbus master.
     *
     * @param response The Modbus response (see @ref Entity::ModbusTcpResponse) which should be sent to the Modbus
     * master.
     * @return A status informing about a successful, failed, ... connection.
     */
    virtual Gateway::ModbusReceiveStatus reply(std::shared_ptr<Entity::ModbusTcpResponse>& response) = 0;

    /**
     * @brief Send a Modbus exception response back to the Modbus master
     *
     * @param mbExceptionCode The Modbus exception code which is used to send a Modbus exception response.
     * @return A status informing about a successful, failed, ... connection.
     */
    virtual Gateway::ModbusReceiveStatus replyException(Entity::ModbusExceptionCode mbExceptionCode) = 0;

    /**
     * @brief Close established connection.
     */
    virtual void close() = 0;

    /**
     * @brief Determines if the connection is up, i.e. the internal slave holds a connection with the external master
     *
     * @return true, if the connection is still up, false otw.
     */
    virtual bool isConnectionUp() = 0;
};

}
