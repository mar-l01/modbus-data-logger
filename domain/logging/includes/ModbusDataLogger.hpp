#pragma once

#include "domain/logging/interfaces/DataLogger.hpp"
#include "domain/logging/interfaces/RealTimeLogger.hpp"

#include <boost/signals2/signal.hpp>

namespace Entity {
// forward declarations
class ModbusTcpRequest;
class ModbusTcpResponse;
}

namespace Logging {

template<typename T>
using SignalEvent = boost::signals2::signal<void(const T&)>;

class ModbusDataLogger
    : public DataLogger
    , public RealTimeLogger
{
public:
    ModbusDataLogger();

    void logModbusRequest(const Entity::ModbusTcpRequest& mbRequest) override;
    void logModbusResponse(const Entity::ModbusTcpResponse& mbResponse) override;

    std::shared_ptr<ScopedConnection> addModbusRequestListener(
      const std::function<void(const Entity::ModbusTcpRequest& mbRequest)>& signalCallback) override;
    std::shared_ptr<ScopedConnection> addModbusResponseListener(
      const std::function<void(const Entity::ModbusTcpResponse& mbResponse)>& signalCallback) override;

private:
    SignalEvent<Entity::ModbusTcpRequest> m_mbRequestEvent;
    SignalEvent<Entity::ModbusTcpResponse> m_mbResponseEvent;
};

}
