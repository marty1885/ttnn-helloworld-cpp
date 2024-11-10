cmake_minimum_required(VERSION 3.16)
project(ttnn-hello CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if("$ENV{TT_METAL_HOME}" STREQUAL "")
    message(FATAL_ERROR "TT_METAL_HOME is not set")
endif()
if("$ENV{ARCH_NAME}" STREQUAL "")
    message(FATAL_ERROR "ARCH_NAME is not set")
endif()

set(NORMALIZED_ARCH_NAME $ENV{ARCH_NAME})
if("$ENV{ARCH_NAME}" STREQUAL "wormhole_b0")
    set(NORMALIZED_ARCH_NAME "wormhole")
endif()

message($ENV{TT_METAL_HOME}/tt_metal/hw/inc/${NORMALIZED_ARCH_NAME})
add_executable(ttnn-hello ttnn-hello.cpp)

target_include_directories(ttnn-hello PRIVATE
    # Metalium
    $ENV{TT_METAL_HOME}
    $ENV{TT_METAL_HOME}/tt_metal
    $ENV{TT_METAL_HOME}/tt_metal/third_party/umd
    $ENV{TT_METAL_HOME}/tt_metal/third_party/umd/device
    $ENV{TT_METAL_HOME}/tt_metal/third_party/fmt
    $ENV{TT_METAL_HOME}/tt_metal/hw/inc/${NORMALIZED_ARCH_NAME}
    $ENV{TT_METAL_HOME}/tt_metal/hw/inc/
    $ENV{TT_METAL_HOME}/tt_metal/third_party/umd/src/firmware/riscv/${NORMALIZED_ARCH_NAME}

    # TTNN
    $ENV{TT_METAL_HOME}/ttnn/cpp
    $ENV{TT_METAL_HOME}/ttnn/cpp/ttnn/deprecated
    $ENV{TT_METAL_HOME}/tt_metal/third_party/magic_enum

)
target_link_directories(ttnn-hello PRIVATE
    $ENV{TT_METAL_HOME}/build/lib
)

# TODO: In the future TTNN can optionally not depend on Python
find_package(Python REQUIRED Development)
target_link_libraries(ttnn-hello PRIVATE
    # Metalium
    tt_metal
    yaml-cpp
    pthread

    # TTNN
    Python::Python
    $ENV{TT_METAL_HOME}/build/lib/_ttnn.so
)

target_compile_definitions(ttnn-hello PRIVATE
    FMT_HEADER_ONLY
)

target_precompile_headers(ttnn-hello PRIVATE pch.hpp)
