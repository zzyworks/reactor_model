cmake_minimum_required(VERSION 3.20)

project(network)

set(LIBRARY_OUTPUT_PATH ${lib_path})

include_directories(${include_path})

aux_source_directory(./ src)

add_library(${lib_network} STATIC ${src})
