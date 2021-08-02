#pragma once

namespace Fixture {

class FixtureModbusGateway
{
public:
    FixtureModbusGateway(bool logOutputToConsole = false);

    void setUp(const int nbReconnections);
    bool isConnectionPossible() const;

private:
    bool m_logOutputToConsole;
    bool m_isConnectionPossible;
};

}
