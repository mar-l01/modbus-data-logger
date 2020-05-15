#include "domain/gateway/includes/ModbusSlaveController.hpp"
#include "domain/gateway/interfaces/ModbusSlave.hpp"

namespace Fixture {

class FixtureModbusSlaveController
{
public:
    FixtureModbusSlaveController();

    void setUp(const int nbIter);

private:
    std::unique_ptr<Gateway::ModbusSlave> m_modbusSlave;
    int m_socket;

    void runRequestResponseLoop(const int nbIter);
};

}
