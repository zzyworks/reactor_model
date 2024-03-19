cmake_minimum_required(VERSION 3.20)

project(client)

aux_source_directory(./ SRC)

include_directories(${INCLUDE_PATH})

link_directories(${LIB_PATH})
link_libraries(${LIB_NETWORK})

set(EXECUTABLE_OUTPUT_PATH ${EXE_PATH})

add_executable(${EXE_CLIENT} ${SRC})
