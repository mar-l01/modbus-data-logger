#pragma once
#include <string>

class HelloModbus
{
public:
    HelloModbus(const std::string& text);
    std::string getModbusText();

private:
    const std::string m_modbusText;
};