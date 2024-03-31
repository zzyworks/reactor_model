cmake_minimum_required(VERSION 3.20)

project(client)

set(EXECUTABLE_OUTPUT_PATH ${exe_path})

include_directories(${include_path})

aux_source_directory(./ src)

link_directories(${lib_path})
link_libraries(${lib_network})

add_executable(${exe_client} ${src})
