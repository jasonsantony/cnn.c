cmake_minimum_required(VERSION 3.10)
project(cnn C)  # C code only :p

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

# set ${SOURCES}
file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.c
)

add_executable(cnn ${SOURCES})

target_link_libraries(cnn m)  # we need to link math library
