cmake_minimum_required(VERSION 3.0.0)

# Configure and Include EBCpp
set(EBCPP_USE_GUI Off)           # On/Off - Using the windows gui functions (only for testing)
set(EBCPP_STATIC On)            # On/Off - Static linking of libc, libc++ and winpthread

set(CMAKE_C_FLAGS "-m32")
set(CMAKE_CXX_FLAGS "-m32")
set(CMAKE_LINK_FLAGS "-m32")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--kill-at -static-libgcc -static-libstdc++")

include(${CMAKE_CURRENT_LIST_DIR}/dep/ebcpp/ebcpp.cmake)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
