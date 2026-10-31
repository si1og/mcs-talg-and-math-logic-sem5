cmake_minimum_required(VERSION 3.16)

project(CellularAutomatonLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_library(automaton
    src/CellularAutomaton.cpp
    src/CellularAutomaton.h
)
target_include_directories(automaton PUBLIC src)

add_executable(cellular_automaton
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/GridWidget.cpp
    src/GridWidget.h
)
target_link_libraries(cellular_automaton PRIVATE automaton Qt6::Widgets)

include(CTest)
if(BUILD_TESTING)
    add_executable(automaton_tests tests/automaton_tests.cpp)
    target_link_libraries(automaton_tests PRIVATE automaton)
    add_test(NAME automaton_tests COMMAND automaton_tests)

    add_executable(automaton_analysis tools/AutomatonAnalysis.cpp)
    target_link_libraries(automaton_analysis PRIVATE automaton)
endif()
