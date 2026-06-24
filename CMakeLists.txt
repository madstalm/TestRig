cmake_minimum_required(VERSION 3.16)
project(TestRigControl VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS src/ui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/galilcontroller.cpp
    src/galilcontroller.h
    src/ui/mainwindow.ui
)

qt_add_executable(TestRigControl
    MANUAL_FINALIZATION
    ${SOURCES}
)

target_include_directories(TestRigControl PRIVATE
    src
    src/ui
)

target_link_libraries(TestRigControl PRIVATE
    Qt6::Widgets
    gclib
    gclibo
)

qt_finalize_executable(TestRigControl)
