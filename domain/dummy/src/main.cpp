#include "HelloModbus.hpp"

#include <iostream>

int main()
{
    HelloModbus helloModbus("Hello Modbus!");
    auto modbusText = helloModbus.getModbusText();
    std::cout << modbusText << std::endl;
}