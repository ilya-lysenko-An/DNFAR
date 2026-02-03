cmake_minimum_required(VERSION 3.16)

project(DnFAR LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

add_executable(phased_array
    src/gui_main.cpp
    src/signal/signal_generator.cpp
    src/dsp/beamforming.cpp
    src/dsp/fft.cpp
    src/dsp/pattern.cpp
)

target_include_directories(phased_array PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(phased_array
    Qt6::Widgets
    Qt6::Charts
)

