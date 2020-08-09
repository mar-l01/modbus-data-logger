#pragma once

#include <boost/signals2/connection.hpp>
#include <functional>
#include <memory>

namespace Entity {
// forward declarations
class ModbusTcpRequest;
class ModbusTcpResponse;
}

namespace Logging {

using ScopedConnection = boost::signals2::scoped_connection;
template<typename T>
using SignalCallback = const std::function<void(const T&)>&;

/**
 * @brief This interface is used for real-time notifications about the exchanged Modbus data.
 * It uses boost's 'signal2'-library to implement the notification to subscribed clients.
 *
 * For further information about this boost library see: https://www.boost.org/doc/libs/1_55_0/doc/html/signals2.html
 */
class RealTimeLogger
{
public:
    /**
     * @brief Add a listener, which gets informed once a new Modbus TCP request was received.
     *
     * @param signalCallback The callback function invoked when informed.
     * @return A shared-pointer instance of a scoped connection which the client must hold as long as it wants to be
     * informed about new data.
     */
    virtual std::shared_ptr<ScopedConnection> addModbusRequestListener(
      SignalCallback<Entity::ModbusTcpRequest> signalCallback) = 0;

    /**
     * @brief Add a listener, which gets informed once a new Modbus TCP response was received.
     *
     * @param signalCallback The callback function invoked when informed.
     * @return A shared-pointer instance of a scoped connection which the client must hold as long as it wants to be
     * informed about new data.
     */
    virtual std::shared_ptr<ScopedConnection> addModbusResponseListener(
      SignalCallback<Entity::ModbusTcpResponse> signalCallback) = 0;
};

}
