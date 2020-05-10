cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)

project(modbus-data-logger)

# build library target
add_library(hellomodbus
    domain/dummy/include/HelloModbus.hpp
    domain/dummy/src/HelloModbus.cpp
)

# files in 'include' should be used whenever library is used
target_include_directories(
  hellomodbus
  PUBLIC
    "domain/dummy/include"
)

# build executable target
add_executable(modbus_data_logger
  domain/dummy/src/main.cpp
)

# executable depends on library
target_link_libraries(modbus_data_logger hellomodbus)
