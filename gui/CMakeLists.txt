cmake_minimum_required(VERSION 3.1.0)

project(ui_application)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core Qml Quick REQUIRED)
find_package(PkgConfig)
pkg_check_modules(LIBMBDL REQUIRED libmbdl>=0.1.0)

# build facade wrapper object-library
add_library(ui_facade OBJECT
    # header files
    "ui/facade/includes/ModbusDataLoggerFacadeUIWrapper.hpp"
    "ui/facade/includes/ModbusDataLoggerThreadController.hpp"
    "ui/facade/includes/ModbusDataLoggerSignals.hpp"

    # source files
    "ui/facade/sources/ModbusDataLoggerFacadeUIWrapper.cpp"
    "ui/facade/sources/ModbusDataLoggerThreadController.cpp"
)
target_compile_options(ui_facade PUBLIC -Wall -Wextra)
target_include_directories(ui_facade PRIVATE
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/.."
    "/usr/include/boost_1_72_0/"
)
target_link_libraries(ui_facade PRIVATE
    Qt5::Core
)

# build views object-library
add_library(ui_views OBJECT
    # header files
    "ui/views/includes/InitialView.hpp"

    # source files
    "ui/views/sources/InitialView.cpp"
)
target_compile_options(ui_views PUBLIC -Wall -Wextra)
target_include_directories(ui_views PRIVATE
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/.."
    "/usr/include/boost_1_72_0/"
)
target_link_libraries(ui_views PRIVATE
    Qt5::Core
)

# build executable
add_executable(ui_application
    $<TARGET_OBJECTS:ui_facade>
    $<TARGET_OBJECTS:ui_views>
    "ui/application/GuiApplication.cpp"
    "ui/resources.qrc"
)
target_compile_options(ui_application PUBLIC -Wall -Wextra)
target_include_directories(ui_application PRIVATE
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/.."
    "/usr/include/boost_1_72_0/"
)
target_link_libraries(ui_application PRIVATE
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
    ${LIBMBDL_LIBDIR}/libmbdl.so.${LIBMBDL_VERSION}
)

##################################################################
# TESTING
##################################################################

if (${CMAKE_ENABLE_TESTING})
  enable_testing()

  ################# START: Google Test Integration #################
  # from: https://github.com/google/googletest/blob/master/googletest/README.md#incorporating-into-an-existing-cmake-project

  # Download and unpack googletest at configure time
  configure_file(../CMakeLists.txt.in googletest-download/CMakeLists.txt)
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

  find_package(Qt5 COMPONENTS Test REQUIRED)

  # build framework unittests
  add_executable(views_test
    $<TARGET_OBJECTS:ui_views>
    "ui/views/testing/TestInitialView.cpp"
  )
  target_include_directories(views_test
    PRIVATE
      "${PROJECT_SOURCE_DIR}"
      "${PROJECT_SOURCE_DIR}/.."
      "/usr/include/boost_1_72_0/"
  )
  target_link_libraries(views_test
    PRIVATE
      Qt5::Core
      Qt5::Test
      ${LIBMBDL_LIBDIR}/libmbdl.so.${LIBMBDL_VERSION}
      gtest
      gmock
      gtest_main
  )

  add_test(NAME views_test COMMAND $<TARGET_FILE:views_test>)

 endif ()
