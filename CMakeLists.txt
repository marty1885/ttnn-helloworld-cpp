cmake_minimum_required(VERSION 3.16)
project(ttnn-hello CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if("$ENV{TT_METAL_HOME}" STREQUAL "")
    message(FATAL_ERROR "TT_METAL_HOME is not set")
endif()
if("$ENV{ARCH_NAME}" STREQUAL "")
    message(FATAL_ERROR "ARCH_NAME is not set")
endif()

message($ENV{TT_METAL_HOME}/tt_metal/third_party/src/firmware/riscv/$ENV{ARCH_NAME})
add_executable(ttnn-hello ttnn-hello.cpp)
target_include_directories(ttnn-hello PRIVATE
    # Metalium
    $ENV{TT_METAL_HOME}
    $ENV{TT_METAL_HOME}/tt_metal
    $ENV{TT_METAL_HOME}/tt_metal/third_party/umd
    $ENV{TT_METAL_HOME}/tt_metal/third_party/fmt
    $ENV{TT_METAL_HOME}/tt_metal/hw/inc/$ENV{ARCH_NAME}
    $ENV{TT_METAL_HOME}/tt_metal/hw/inc/
    $ENV{TT_METAL_HOME}/tt_metal/third_party/umd/src/firmware/riscv/$ENV{ARCH_NAME}

    # TTNN
    $ENV{TT_METAL_HOME}/ttnn/cpp
    $ENV{TT_METAL_HOME}/tt_eager
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
    c++abi
    c++

    # TTNN
    tt_eager
    Python::Python
    $ENV{TT_METAL_HOME}/build/lib/_ttnn.so
)

target_compile_definitions(ttnn-hello PRIVATE
    FMT_HEADER_ONLY
)

target_compile_options(ttnn-hello PRIVATE -stdlib=libc++)

target_precompile_headers(ttnn-hello PRIVATE pch.hpp)