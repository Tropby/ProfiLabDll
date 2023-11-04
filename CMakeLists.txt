cmake_minimum_required(VERSION 3.0.0)
project(ExampleProfilabDll VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(WINDOWS_VERSION 0x0A00) 
add_definitions(-DWINVER=${WINDOWS_VERSION})
add_definitions(-D_WIN32_WINNT=${WINDOWS_VERSION})

include_directories(dep/ebcpp/src)
include_directories(include)

add_library(ExampleProfilabDll SHARED example/example.cpp)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--kill-at -static-libgcc -static-libstdc++")

target_compile_definitions(ExampleProfilabDll PRIVATE WIN_EXPORT)