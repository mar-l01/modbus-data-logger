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

using ConnectionPointer = std::shared_ptr<boost::signals2::scoped_connection>;

class RealTimeLogger
{
public:
    virtual ConnectionPointer addModbusRequestListener(
      const std::function<void(const Entity::ModbusTcpRequest& mbRequest)>& signalCallback) = 0;

    virtual ConnectionPointer addModbusResponseListener(
      const std::function<void(const Entity::ModbusTcpResponse& mbResponse)>& signalCallback) = 0;
};

}
