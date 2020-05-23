cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

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
  "domain/gateway/includes/ModbusSlaveController.hpp"
  "domain/gateway/includes/ModbusGateway.hpp"
  "domain/gateway/includes/ModbusTcpMessageFrame.hpp"
  "domain/gateway/includes/ModbusTcpRequest.hpp"
  "domain/gateway/includes/ModbusTcpResponse.hpp"
  "domain/gateway/interfaces/ModbusSlave.hpp"
  "domain/gateway/interfaces/ModbusRequestController.hpp"

  # source files
  "domain/gateway/sources/ModbusDataMapping.cpp"
  "domain/gateway/sources/ModbusSlaveFactory.cpp"
  "domain/gateway/sources/ModbusSlaveController.cpp"
  "domain/gateway/sources/ModbusGateway.cpp"
  "domain/gateway/sources/ModbusTcpMessageFrame.cpp"
  "domain/gateway/sources/ModbusTcpRequest.cpp"
  "domain/gateway/sources/ModbusTcpResponse.cpp"
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

##################################################################
# TESTING
##################################################################

if (${CMAKE_ENABLE_TESTING})
  enable_testing()

  ################# START: Google Test Integration #################
  # from: https://github.com/google/googletest/blob/master/googletest/README.md#incorporating-into-an-existing-cmake-project

  # Download and unpack googletest at configure time
  configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
  if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
  endif()
  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
  if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
  endif()

  # Add googletest directly to our build. This defines
  # the gtest and gtest_main targets.
  add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                  ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                  EXCLUDE_FROM_ALL)

  #################### END: Google Test Integration ####################

  # build framework unittests
  add_executable(framework_test
    $<TARGET_OBJECTS:mdl_framework>
    $<TARGET_OBJECTS:mdl_gateway>
    "domain/framework/testing/TestLibModbusSlave.cpp"
  )
  target_include_directories(framework_test PRIVATE "${PROJECT_SOURCE_DIR}")
  target_link_libraries(framework_test
    PRIVATE
      /usr/local/lib/libmodbus.so
      gtest
      gmock
      gtest_main
  )

  add_test(NAME framework_test COMMAND $<TARGET_FILE:framework_test>)

  # build gateway unittests
  add_executable(gateway_test
    $<TARGET_OBJECTS:mdl_framework>
    $<TARGET_OBJECTS:mdl_gateway>
    "domain/gateway/testing/TestModbusDataMapping.cpp"
    "domain/gateway/testing/TestModbusSlaveFactory.cpp"
    "domain/gateway/testing/TestModbusSlaveController.cpp"
    "domain/gateway/testing/TestModbusTcpMessageFrame.cpp"
    "domain/gateway/testing/TestModbusTcpRequest.cpp"
    "domain/gateway/testing/TestModbusTcpResponse.cpp"
  )
  target_include_directories(gateway_test PRIVATE "${PROJECT_SOURCE_DIR}")
  target_sources(gateway_test
    PRIVATE
      "domain/gateway/testing/gmock/MockModbusSlave.hpp"
      "domain/gateway/testing/gmock/MockModbusRequestController.hpp"
  )
  target_link_libraries(gateway_test
    PRIVATE
      /usr/local/lib/libmodbus.so
      gtest
      gmock
      gtest_main
  )

  add_test(NAME gateway_test COMMAND $<TARGET_FILE:gateway_test>)

   # build integrationtest
  add_executable(integrationtest
    $<TARGET_OBJECTS:mdl_framework>
    $<TARGET_OBJECTS:mdl_gateway>
    "integrationtests/TestModbusSlaveCommunication.cpp"
  )
  target_include_directories(integrationtest PRIVATE "${PROJECT_SOURCE_DIR}")
  target_sources(integrationtest
    PRIVATE
      "integrationtests/fixtures/sources/FixtureModbusSlaveController.cpp"
      "integrationtests/fixtures/sources/FixtureExternalModbusMaster.cpp"
)
  target_link_libraries(integrationtest
    PRIVATE
      /usr/local/lib/libmodbus.so
      gtest
      gmock
      gtest_main
  )

  add_test(NAME integrationtest COMMAND $<TARGET_FILE:integrationtest>)
endif ()
