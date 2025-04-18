cmake_minimum_required(VERSION 3.16)
project(ttnn-hello CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(USE_LIBCPP OFF)

if("$ENV{TT_METAL_HOME}" STREQUAL "")
    message(FATAL_ERROR "TT_METAL_HOME is not set")
endif()

if(DEFINED ENV{CMAKE_C_COMPILER} AND DEFINED ENV{CMAKE_CXX_COMPILER})
    message(STATUS "Setting C and C++ compiler from environment variables")
    set(CMAKE_C_COMPILER $ENV{CMAKE_C_COMPILER})
    set(CMAKE_CXX_COMPILER $ENV{CMAKE_CXX_COMPILER})
endif()

if(CMAKE_CXX_COMPILER AND CMAKE_C_COMPILER)
    message(STATUS "Using specifed C++ compiler: ${CMAKE_CXX_COMPILER}")
    message(STATUS "Using specifed C compiler: ${CMAKE_C_COMPILER}")
else()
    message(STATUS "No C or C++ compiler specified, using system default compiler")
endif()

if(NOT DEFINED CPM_SOURCE_CACHE)
    message(STATUS "Setting CPM_SOURCE_CACHE to ${PROJECT_SOURCE_DIR}/.cpmcache")
    set(CPM_SOURCE_CACHE "${PROJECT_SOURCE_DIR}/.cpmcache")
else()
    message(STATUS "CPM_SOURCE_CACHE is set to: ${CPM_SOURCE_CACHE}")
endif()

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CPM)

if(CMAKE_VERSION VERSION_LESS 3.25)
    add_subdirectory(dependencies EXCLUDE_FROM_ALL)
else()
    add_subdirectory(dependencies EXCLUDE_FROM_ALL SYSTEM)
endif()

add_executable(ttnn-hello ttnn-hello.cpp)

target_include_directories(ttnn-hello PRIVATE
    $ENV{TT_METAL_HOME}
    $ENV{TT_METAL_HOME}/tt_metal
    $ENV{TT_METAL_HOME}/tt_metal/api/
    $ENV{TT_METAL_HOME}/tt_metal/include
    $ENV{TT_METAL_HOME}/tt_metal/hostdevcommon/api
    $ENV{TT_METAL_HOME}/tt_metal/third_party/umd
    $ENV{TT_METAL_HOME}/tt_metal/third_party/umd/device
    $ENV{TT_METAL_HOME}/tt_metal/third_party/umd/device/api/
    $ENV{TT_METAL_HOME}/tt_metal/third_party/taskflow/3rd-party/
    $ENV{TT_METAL_HOME}/tt_metal/third_party/tracy/public/
    $ENV{TT_METAL_HOME}/tt_metal/hw/inc/
    $ENV{TT_METAL_HOME}/tt_stl/

    # TTNN
    $ENV{TT_METAL_HOME}/ttnn
    $ENV{TT_METAL_HOME}/ttnn/cpp
    $ENV{TT_METAL_HOME}/tt_metal/third_party/magic_enum
)

target_link_directories(ttnn-hello PRIVATE
    $ENV{TT_METAL_HOME}/build/tt_metal
)

target_link_libraries(ttnn-hello PRIVATE
    fmt
    magic_enum
    Reflect::Reflect
    yaml-cpp
    Boost::core
    Boost::container
    tt_metal
    $ENV{TT_METAL_HOME}/build/ttnn/_ttnn.so
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND USE_LIBCPP)
    target_compile_options(ttnn-hello PRIVATE -stdlib=libc++)
endif()

target_compile_definitions(ttnn-hello PRIVATE
    FMT_HEADER_ONLY
)

target_precompile_headers(ttnn-hello PRIVATE pch.hpp)
