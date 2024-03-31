cmake_minimum_required(VERSION 3.20)

project(reactor_model)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(EXE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(LIB_NETWORK network)

set(EXE_SERVER server)
set(EXE_CLIENT client)

add_subdirectory(network)
add_subdirectory(tests)
add_subdirectory(client)
add_subdirectory(server)
