#include "HelloModbus.hpp"

#include <iostream>

HelloModbus::HelloModbus(const std::string& text)
    : m_modbusText(text)
{}

std::string HelloModbus::getModbusText()
{
    return m_modbusText;
}