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
)
target_link_libraries(ui_views PRIVATE
    Qt5::Core
)

# build executable
add_executable(ui_application
    $<TARGET_OBJECTS:ui_views>
    "ui/application/GuiApplication.cpp"
    "resources.qrc"
)
target_compile_options(ui_application PUBLIC -Wall -Wextra)
target_include_directories(ui_application PRIVATE
    "${PROJECT_SOURCE_DIR}"
)
target_link_libraries(ui_application PRIVATE
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
)
