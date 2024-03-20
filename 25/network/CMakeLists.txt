cmake_minimum_required(VERSION 3.20)

project(network)

set(LIBRARY_OUTPUT_PATH ${LIB_PATH})

include_directories(${INCLUDE_PATH})

aux_source_directory(./ SRC)

add_library(${LIB_NETWORK} STATIC ${SRC})
