#pragma once
#include <string>

class HelloModbus
{
public:
    explicit HelloModbus(const std::string& text);
    std::string getModbusText();

private:
    const std::string m_modbusText;
};
