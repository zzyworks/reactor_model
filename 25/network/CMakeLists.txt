cmake_minimum_required(VERSION 3.20)

project(network)

aux_source_directory(./ SRC)

include_directories(${INCLUDE_PATH})

set(LIBRARY_OUTPUT_PATH ${LIB_PATH})

add_library(${LIB_NETWORK} STATIC ${SRC})
