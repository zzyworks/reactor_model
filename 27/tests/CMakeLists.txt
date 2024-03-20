cmake_minimum_required(VERSION 3.20)

project(tests)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${INCLUDE_PATH})

aux_source_directory(./ SRC)

link_directories(${LIB_PATH})
link_libraries(${LIB_NETWORK})


add_executable(${EXE_TEST1} ${SRC}) 
