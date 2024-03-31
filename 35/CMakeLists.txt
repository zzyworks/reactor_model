cmake_minimum_required(VERSION 3.20)

project(reactor_model)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(lib_path ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(exe_path ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(include_path ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(lib_network network)
set(exe_server server)
set(exe_client client)

add_subdirectory(network)
add_subdirectory(client)
add_subdirectory(server)
