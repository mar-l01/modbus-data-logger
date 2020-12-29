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

add_executable(ui_application
    application/GuiApplication.cpp
    resources.qrc
)

target_link_libraries(ui_application Qt5::Core Qt5::Qml Qt5::Quick)
