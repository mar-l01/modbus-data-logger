cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)

project(modbus-data-logger-project)

# all sources files
file(GLOB SOURCES
  "${PROJECT_SOURCE_DIR}/domain/framework/sources/*.cpp"
  "${PROJECT_SOURCE_DIR}/domain/gateway/datatypes/sources/*.cpp"
  "${PROJECT_SOURCE_DIR}/domain/gateway/sources/*.cpp"
  "${PROJECT_SOURCE_DIR}/domain/application/*.cpp"
)

# build executable target
add_executable(modbus_data_logger ${SOURCES})

target_include_directories(modbus_data_logger
  PRIVATE
    "${PROJECT_SOURCE_DIR}"
)

# link libmodbus library
target_link_libraries(modbus_data_logger /usr/local/lib/libmodbus.so)
