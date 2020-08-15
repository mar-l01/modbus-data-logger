cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(modbus-data-logger-project)

##################################################################
# BUILD APPLICATION
##################################################################

# add preprocessor macros for logging (debug -> DEBUG, release -> ERROR)
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  add_compile_definitions(DEBUG)
  add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_ERROR)
endif()

# logging library, static linked
find_package(spdlog REQUIRED)

# build framework object-library
add_library(mdl_framework OBJECT
  # header files
  "domain/framework/includes/LibModbusSlave.hpp"
  "domain/framework/includes/LibModbusMaster.hpp"
  "domain/framework/interfaces/ConfigurationFileReader.hpp"
  "domain/framework/includes/JsonFileReader.hpp"
  "domain/framework/includes/FileReaderFactory.hpp"
  "domain/framework/includes/SpdlogFileController.hpp"
  "domain/framework/includes/FileLoggerControllerFactory.hpp"

  # source files
  "domain/framework/sources/LibModbusSlave.cpp"
  "domain/framework/sources/LibModbusMaster.cpp"
  "domain/framework/sources/JsonFileReader.cpp"
  "domain/framework/sources/FileReaderFactory.cpp"
  "domain/framework/sources/SpdlogFileController.cpp"
  "domain/framework/sources/FileLoggerControllerFactory.cpp"
)
target_compile_options(mdl_framework PUBLIC -Wall -Wextra)
target_include_directories(mdl_framework PRIVATE
  "${PROJECT_SOURCE_DIR}"
  "/usr/include/better-enums/"
)
target_link_libraries(mdl_framework PRIVATE ${CMAKE_USR_LIB_ARCH}/libmodbus.so)

# build gateway object-library
add_library(mdl_gateway OBJECT
  # header files
  "domain/gateway/includes/ModbusComponentsFactory.hpp"
  "domain/gateway/includes/ModbusSlaveController.hpp"
  "domain/gateway/includes/ModbusMasterController.hpp"
  "domain/gateway/includes/ModbusGateway.hpp"
  "domain/gateway/interfaces/ModbusSlave.hpp"
  "domain/gateway/interfaces/ModbusMaster.hpp"
  "domain/gateway/interfaces/ModbusRequestController.hpp"
  "domain/gateway/interfaces/ModbusResponseController.hpp"

  # source files
  "domain/gateway/sources/ModbusComponentsFactory.cpp"
  "domain/gateway/sources/ModbusSlaveController.cpp"
  "domain/gateway/sources/ModbusMasterController.cpp"
  "domain/gateway/sources/ModbusGateway.cpp"
)
target_compile_options(mdl_gateway PUBLIC -Wall -Wextra)
target_include_directories(mdl_gateway PRIVATE
  "${PROJECT_SOURCE_DIR}"
  "/usr/include/better-enums/"
)

# build entity object-library
add_library(mdl_entity OBJECT
  # header files
  "domain/entity/includes/ModbusDataMapping.hpp"
  "domain/entity/includes/ModbusTcpMessageFrame.hpp"
  "domain/entity/includes/ModbusTcpRequest.hpp"
  "domain/entity/includes/ModbusTcpResponse.hpp"
  "domain/entity/includes/ModbusTcpConstants.hpp"
  "domain/entity/includes/ModbusConfiguration.hpp"
  "domain/entity/includes/ModbusLoggerConfiguration.hpp"
  "domain/entity/interfaces/ModbusDataLog.hpp"

  # source files
  "domain/entity/sources/ModbusDataMapping.cpp"
  "domain/entity/sources/ModbusTcpMessageFrame.cpp"
  "domain/entity/sources/ModbusTcpRequest.cpp"
  "domain/entity/sources/ModbusTcpResponse.cpp"
  "domain/entity/sources/ModbusConfiguration.cpp"
  "domain/entity/sources/ModbusLoggerConfiguration.cpp"
)
target_compile_options(mdl_entity PUBLIC -Wall -Wextra)
target_include_directories(mdl_entity PRIVATE
  "${PROJECT_SOURCE_DIR}"
  "/usr/include/better-enums/"
)

# build utility object-library
add_library(mdl_utility OBJECT
  # header files
  "domain/utility/interfaces/Timer.hpp"
  "domain/utility/includes/TimerImpl.hpp"
  "domain/utility/includes/TimerFactory.hpp"

  # source files
  "domain/utility/sources/TimerImpl.cpp"
  "domain/utility/sources/TimerFactory.cpp"
)
target_compile_options(mdl_utility PUBLIC -Wall -Wextra)
target_include_directories(mdl_utility PRIVATE
  "${PROJECT_SOURCE_DIR}"
)

# build logging object-library
add_library(mdl_logging OBJECT
  # header files
  "domain/logging/interfaces/DataLogger.hpp"
  "domain/logging/interfaces/RealTimeLogger.hpp"
  "domain/logging/interfaces/FileLogger.hpp"
  "domain/logging/interfaces/FileLoggerController.hpp"
  "domain/logging/includes/ModbusDataLogger.hpp"

  # source files
  "domain/logging/sources/ModbusDataLogger.cpp"
)
target_compile_options(mdl_logging PUBLIC -Wall -Wextra)
target_include_directories(mdl_logging PRIVATE
  "${PROJECT_SOURCE_DIR}"
  "/usr/include/boost_1_72_0/"
)

# build executable target
add_executable(modbus_data_logger
  $<TARGET_OBJECTS:mdl_framework>
  $<TARGET_OBJECTS:mdl_gateway>
  $<TARGET_OBJECTS:mdl_entity>
  $<TARGET_OBJECTS:mdl_utility>
  $<TARGET_OBJECTS:mdl_logging>
  "domain/application/sources/ConsoleApplication.cpp"
)
target_compile_options(modbus_data_logger PUBLIC -Wall -Wextra -pthread)
target_include_directories(modbus_data_logger PRIVATE
  "${PROJECT_SOURCE_DIR}"
  "/usr/include/better-enums/"
  "/usr/include/nlohmann/"
  "/usr/include/boost_1_72_0/"
)
target_link_libraries(modbus_data_logger PRIVATE
  ${CMAKE_USR_LIB_ARCH}/libmodbus.so
  ${CMAKE_USR_LIB_ARCH}/libpthread.so
)

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
    $<TARGET_OBJECTS:mdl_entity>
    "domain/framework/testing/TestLibModbusSlave.cpp"
    "domain/framework/testing/TestLibModbusMaster.cpp"
    "domain/framework/testing/TestFileReaderFactory.cpp"
    "domain/framework/testing/TestJsonFileReader.cpp"
    "domain/framework/testing/TestSpdlogFileController.cpp"
    "domain/framework/testing/TestFileLoggerControllerFactory.cpp"
  )
  target_include_directories(framework_test PRIVATE
    "${PROJECT_SOURCE_DIR}"
    "/usr/include/better-enums/"
  )
  target_sources(framework_test
    PRIVATE
      "domain/framework/testing/gmock/MockConfigurationFileReader.hpp"
  )
  target_link_libraries(framework_test
    PRIVATE
      ${CMAKE_USR_LIB_ARCH}/libmodbus.so
      gtest
      gmock
      gtest_main
      stdc++fs # std::filesystem
  )

  add_test(NAME framework_test COMMAND $<TARGET_FILE:framework_test>)

  # build gateway unittests
  add_executable(gateway_test
    $<TARGET_OBJECTS:mdl_framework>
    $<TARGET_OBJECTS:mdl_gateway>
    $<TARGET_OBJECTS:mdl_entity>
    "domain/gateway/testing/TestModbusComponentsFactory.cpp"
    "domain/gateway/testing/TestModbusSlaveController.cpp"
    "domain/gateway/testing/TestModbusMasterController.cpp"
    "domain/gateway/testing/TestModbusGateway.cpp"
  )
  target_include_directories(gateway_test PRIVATE
    "${PROJECT_SOURCE_DIR}"
    "/usr/include/better-enums/"
  )
  target_sources(gateway_test
    PRIVATE
      "domain/gateway/testing/gmock/MockModbusSlave.hpp"
      "domain/gateway/testing/gmock/MockModbusMaster.hpp"
      "domain/gateway/testing/gmock/MockModbusRequestController.hpp"
      "domain/gateway/testing/gmock/MockModbusResponseController.hpp"
  )
  target_link_libraries(gateway_test
    PRIVATE
      ${CMAKE_USR_LIB_ARCH}/libmodbus.so
      gtest
      gmock
      gtest_main
  )

  add_test(NAME gateway_test COMMAND $<TARGET_FILE:gateway_test>)

  # build entity unittests
  add_executable(entity_test
    $<TARGET_OBJECTS:mdl_entity>
    "domain/entity/testing/TestModbusDataMapping.cpp"
    "domain/entity/testing/TestModbusTcpMessageFrame.cpp"
    "domain/entity/testing/TestModbusTcpRequest.cpp"
    "domain/entity/testing/TestModbusTcpResponse.cpp"
  )
  target_include_directories(entity_test PRIVATE
    "${PROJECT_SOURCE_DIR}"
    "/usr/include/better-enums/"
  )
  target_link_libraries(entity_test
    PRIVATE
      gtest
      gtest_main
  )

  add_test(NAME entity_test COMMAND $<TARGET_FILE:entity_test>)

  # build utility unittests
  add_executable(utility_test
    $<TARGET_OBJECTS:mdl_utility>
    "domain/utility/testing/TestTimer.cpp"
    "domain/utility/testing/TestTimerFactory.cpp"
  )
  target_include_directories(utility_test PRIVATE
    "${PROJECT_SOURCE_DIR}"
  )
  target_link_libraries(utility_test
    PRIVATE
      gtest
      gmock
      gtest_main
  )

  add_test(NAME utility_test COMMAND $<TARGET_FILE:utility_test>)

  # build logging unittests
  add_executable(logging_test
    $<TARGET_OBJECTS:mdl_logging>
    $<TARGET_OBJECTS:mdl_entity>
    "domain/logging/testing/TestModbusDataLogger.cpp"
  )
  target_include_directories(logging_test PRIVATE
    "${PROJECT_SOURCE_DIR}"
    "/usr/include/boost_1_72_0/"
  )
  target_sources(logging_test
    PRIVATE
      "domain/logging/testing/gmock/MockDataLogger.hpp"
      "domain/logging/testing/gmock/MockFileLogger.hpp"
      "domain/logging/testing/gmock/MockRealTimeLogger.hpp"
      "domain/logging/testing/gmock/MockFileLoggerController.hpp"
  )
  target_link_libraries(logging_test
    PRIVATE
      gtest
      gmock
      gtest_main
  )

  add_test(NAME logging_test COMMAND $<TARGET_FILE:logging_test>)

   # build integrationtest
  add_executable(integrationtest
    $<TARGET_OBJECTS:mdl_framework>
    $<TARGET_OBJECTS:mdl_gateway>
    $<TARGET_OBJECTS:mdl_entity>
    $<TARGET_OBJECTS:mdl_utility>
    $<TARGET_OBJECTS:mdl_logging>
    "integrationtests/TestModbusCommunication.cpp"
    "integrationtests/TestModbusReconnection.cpp"
    "integrationtests/TestModbusLogging.cpp"
  )
  target_include_directories(integrationtest PRIVATE
    "${PROJECT_SOURCE_DIR}"
    "/usr/include/better-enums/"
    "/usr/include/boost_1_72_0/"
  )
  target_sources(integrationtest
    PRIVATE
      "integrationtests/fixtures/sources/FixtureExternalModbusMaster.cpp"
      "integrationtests/fixtures/sources/FixtureExternalModbusSlave.cpp"
      "integrationtests/fixtures/sources/FixtureModbusGateway.cpp"
      "integrationtests/fixtures/sources/FixtureDataLogger.cpp"
)
  target_link_libraries(integrationtest
    PRIVATE
      ${CMAKE_USR_LIB_ARCH}/libmodbus.so
      gtest
      gmock
      gtest_main
  )

  add_test(NAME integrationtest COMMAND $<TARGET_FILE:integrationtest>)
endif ()
