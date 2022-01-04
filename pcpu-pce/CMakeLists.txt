cmake_minimum_required(VERSION 3.13)
project(PCPUEMU)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2 -g")

include_directories(include)

set(sources ${sources} 
    src/main.cpp
    src/vga.cpp
    src/core.cpp
    src/loader.cpp
    src/debugger.cpp
    src/sd.cpp
)

add_executable(pce ${sources})
target_link_libraries(pce sfml-graphics sfml-window sfml-system)