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

class RealTimeLogger
{
public:
    virtual std::shared_ptr<ScopedConnection> addModbusRequestListener(
      const std::function<void(const Entity::ModbusTcpRequest& mbRequest)>& signalCallback) = 0;

    virtual std::shared_ptr<ScopedConnection> addModbusResponseListener(
      const std::function<void(const Entity::ModbusTcpResponse& mbResponse)>& signalCallback) = 0;
};

}
