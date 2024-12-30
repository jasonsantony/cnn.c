cmake_minimum_required(VERSION 3.10)
project(cnn C)  # C code only :p

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Set ${SOURCES}
file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.c
)

add_executable(cnn ${SOURCES})

target_link_libraries(cnn m)  # Link the math library

# -Wall -Wextra -Werror -fopenmp
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Werror -fopenmp)
elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /WX /openmp)
endif()

# debug target
add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug -B ${CMAKE_BINARY_DIR}/debug -S ${CMAKE_SOURCE_DIR} && ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/debug --config Debug
    COMMENT "Building in Debug mode with AddressSanitizer"
)

# add ASan flags for debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(cnn PRIVATE -fsanitize=address -g -O0)
        target_link_options(cnn PRIVATE -fsanitize=address)
    elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    message(WARNING "AddressSanitizer is not natively supported on MSVC.")
    endif()
endif()