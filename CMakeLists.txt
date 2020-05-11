cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)

project(modbus-data-logger-project)

##################################################################
# APPLICATION
##################################################################

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

# link libmodbus library and google-test
target_link_libraries(modbus_data_logger
  /usr/local/lib/libmodbus.so
  gtest
  gmock
)

##################################################################
# TESTING
##################################################################

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

# all sources files required for testing
file(GLOB TEST_SOURCES
  "${PROJECT_SOURCE_DIR}/domain/framework/sources/*.cpp"
  "${PROJECT_SOURCE_DIR}/domain/gateway/datatypes/sources/*.cpp"
  "${PROJECT_SOURCE_DIR}/domain/gateway/sources/*.cpp"
  "${PROJECT_SOURCE_DIR}/domain/gateway/testing/*.cpp"
  "${PROJECT_SOURCE_DIR}/domain/framework/testing/*.cpp"
)

# build executable unittest-target
add_executable(mdl_unittests ${TEST_SOURCES})

target_include_directories(mdl_unittests
  PRIVATE
    "${PROJECT_SOURCE_DIR}"
)

# link libmodbus library and google-test-main
target_link_libraries(mdl_unittests
  /usr/local/lib/libmodbus.so
  gtest
  gmock
  gtest_main
)
