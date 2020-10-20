cmake_minimum_required(VERSION 3.1.0)

project(ui_application)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Quick REQUIRED)

add_executable(ui_application
    application/GuiApplication.cpp
)

target_link_libraries(ui_application Qt5::Widgets Qt5::Quick)
