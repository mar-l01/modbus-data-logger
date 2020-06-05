#pragma once

namespace Fixture {

class FixtureModbusGateway
{
public:
    FixtureModbusGateway();

    void setUp(const int nbReconnections);
};

}
