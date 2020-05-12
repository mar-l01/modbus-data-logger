cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)

project(modbus-data-logger-project)

##################################################################
# BUILD APPLICATION
##################################################################

# build framework object-library
add_library(mdl_framework OBJECT
  # header files
  "domain/framework/includes/LibModbusSlave.hpp"

  # source files
  "domain/framework/sources/LibModbusSlave.cpp"
)
target_compile_options(mdl_framework PUBLIC -Wall -Wextra)
target_include_directories(mdl_framework PRIVATE "${PROJECT_SOURCE_DIR}")
target_link_libraries(mdl_framework PRIVATE /usr/local/lib/libmodbus.so)

# build gateway object-library
add_library(mdl_gateway OBJECT
  # header files
  "domain/gateway/includes/ModbusDataMapping.hpp"
  "domain/gateway/includes/ModbusSlaveFactory.hpp"
  "domain/gateway/includes/ModbusSlaveHandler.hpp"
  "domain/gateway/interfaces/ModbusSlave.hpp"

  # source files
  "domain/gateway/sources/ModbusDataMapping.cpp"
  "domain/gateway/sources/ModbusSlaveFactory.cpp"
  "domain/gateway/sources/ModbusSlaveHandler.cpp"
)
target_compile_options(mdl_gateway PUBLIC -Wall -Wextra)
target_include_directories(mdl_gateway PRIVATE "${PROJECT_SOURCE_DIR}")

# build executable target
add_executable(modbus_data_logger
  $<TARGET_OBJECTS:mdl_framework>
  $<TARGET_OBJECTS:mdl_gateway>
  "domain/application/sources/ConsoleApplication.cpp"
)
target_compile_options(modbus_data_logger PUBLIC -Wall -Wextra)
target_include_directories(modbus_data_logger PRIVATE "${PROJECT_SOURCE_DIR}")
target_link_libraries(modbus_data_logger PRIVATE /usr/local/lib/libmodbus.so)
