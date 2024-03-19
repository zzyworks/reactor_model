cmake_minimum_required(VERSION 3.20)

project(client)

set(EXECUTABLE_OUTPUT_PATH ${EXE_PATH})

include_directories(${INCLUDE_PATH})

aux_source_directory(./ SRC)

link_directories(${LIB_PATH})
link_libraries(${LIB_NETWORK})

add_executable(${EXE_CLIENT} ${SRC})
