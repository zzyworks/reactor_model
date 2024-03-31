cmake_minimum_required(VERSION 3.20)

project(tests)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(EXE_TEST1 test_thread_pool)
set(EXE_TEST2 test_smart_ptr)

include_directories(${INCLUDE_PATH})

aux_source_directory(./test_thread_pool.cpp TEST1_SRC)
aux_source_directory(./test_thread_pool.cpp TEST1_SRC)

link_directories(${LIB_PATH})
link_libraries(${LIB_NETWORK})

add_executable(${EXE_TEST1} ${SRC}) 
add_executable(${EXE_TEST2} ${SRC}) 
