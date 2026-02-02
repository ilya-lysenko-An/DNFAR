cmake_minimum_required(VERSION 3.16)

project(DnFAR LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(phased_array
    main.cpp
    dsp.cpp
)

target_include_directories(phased_array PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)