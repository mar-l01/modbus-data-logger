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

class RealTimeLogger
{
public:
    virtual std::shared_ptr<ScopedConnection> addModbusRequestListener(
      SignalCallback<Entity::ModbusTcpRequest> signalCallback) = 0;

    virtual std::shared_ptr<ScopedConnection> addModbusResponseListener(
      SignalCallback<Entity::ModbusTcpResponse> signalCallback) = 0;
};

}
