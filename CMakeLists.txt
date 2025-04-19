#==============================================================================
cmake_minimum_required(VERSION 3.16)
project(ARGPARSE VERSION "0.0.1")
#==============================================================================
# Build type
if(NOT DEFINED CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE RELEASE)
endif()
## Relase
if (CMAKE_BUILD_TYPE MATCHES RELEASE)
        set(CMAKE_CXX_FLAGS "-O3")
endif()
## Debug
if (CMAKE_BUILD_TYPE MATCHES DEBUG)
        set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Wpedantic -Wconversion")
endif()
#==============================================================================
# argparse
set(argparse_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/argument.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cpp)

set(argparse_headers ${CMAKE_CURRENT_SOURCE_DIR}/src/argument.hpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.hpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/src/argparse.hpp)

add_library(argparse SHARED ${argparse_sources})

target_include_directories(argparse PUBLIC 
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# argparse needs c++23 (for std::format)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(argparse PRIVATE "-std=c++23")
else()
        message(FATAL_ERROR "Unsupported compiler: " CMAKE_CXX_COMPILER_ID)
endif()
#==============================================================================
# Installation
include(GNUInstallDirs)

install(FILES ${argparse_headers} 
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS argparse
        EXPORT argparseTargets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(EXPORT argparseTargets
        FILE argparseTargets.cmake
        NAMESPACE argparse::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/argparse)

# Create and install configuration file (argparseConfig.cmake)
include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in 
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/argparseConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/argparse)

install(FILES
       ${CMAKE_CURRENT_SOURCE_DIR}/cmake/argparseConfig.cmake
       DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/argparse)