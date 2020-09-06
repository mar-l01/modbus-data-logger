#pragma once

#include "domain/application/interfaces/ModbusDataLoggerFacade.hpp"

#include <memory>

namespace Fixture {

class FixtureModbusDataLoggerFacade
{
public:
    void setUp();

    std::shared_ptr<Application::ModbusDataLoggerFacade> getFixture() const;

private:
    std::shared_ptr<Application::ModbusDataLoggerFacade> m_mbDataLoggerFacade;
};

} // namespace Fixture
