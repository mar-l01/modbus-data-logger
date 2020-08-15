#pragma once

namespace Fixture {

class FixtureModbusGateway
{
public:
    FixtureModbusGateway(bool logOutputToConsole = false);

    void setUp(const int nbReconnections);

private:
    bool m_logOutputToConsole;
};

}
