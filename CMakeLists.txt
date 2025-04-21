#==============================================================================
cmake_minimum_required(VERSION 3.18)
project(ARGPARSE VERSION "0.0.1")
#==============================================================================
# Build type
if(NOT DEFINED CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE RELEASE)
endif()
#==============================================================================
# Create the library
add_library(argparse SHARED)

set(argparse_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/argument.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cpp)

target_sources(argparse PRIVATE ${argparse_sources})

set(argparse_headers ${CMAKE_CURRENT_SOURCE_DIR}/src/argument.hpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.hpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/src/argparse.hpp)

target_include_directories(argparse PUBLIC 
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
#==============================================================================
# Set compiler flags
## Relase flags
if (CMAKE_BUILD_TYPE MATCHES RELEASE)
        list(APPEND argparse_cxx_flags -O3)
endif()
## Debug flags
if (CMAKE_BUILD_TYPE MATCHES DEBUG)
        list(APPEND  argparse_cxx_flags -g;-Wall;-Wextra;-Wpedantic;-Wconversion)
endif()
target_compile_options(argparse PUBLIC ${argparse_cxx_flags})
#==============================================================================
# C++ standard
set_target_properties(argparse PROPERTIES 
                      CXX_STANDARD 23
                      CXX_STANDARD_REQUIRED ON)
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