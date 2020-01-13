cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

# cmake setup
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# general setup
# TODO: insert your group number here
set(GROUP_NUMBER "00")

project(its2019 C CXX)
enable_testing()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
      "Choose the type of build." FORCE)
endif()

set(VALGRIND_ARGUMENTS --track-origins=yes --tool=memcheck --leak-check=yes --leak-resolution=high --show-reachable=yes)

# doygen setup
set(DOXYGEN_CONFIG_IN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/doc/template.doxy.in")
set(DEFAULT_DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc")

include(doxygenSupport)
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(FindCheck)

# set required C and C++ standard version
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# check compiler flags
foreach(flag IN ITEMS "Wall" "fdiagnostics-color=auto" "Wshadow=local" "Wextra" "pedantic"
    "Werror=vla" "Werror=implicit-function-declaration")
  check_c_compiler_flag("-${flag}" CC_SUPPORTS_${flag})
  check_cxx_compiler_flag("-${flag}" CXX_SUPPORTS_${flag})

  if(CC_SUPPORTS_${flag})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -${flag}")
  endif()
  if(CXX_SUPPORTS_${flag})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -${flag}")
  endif()
endforeach(flag)

find_package(Check 0.10 REQUIRED)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads)

if(CMAKE_USE_PTHREADS_INIT)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.62 REQUIRED COMPONENTS system)
set(PROTOCOLS "true")
if( PROTOCOLS )
  message("linking against reference Bigint lib")
  add_definitions("-DPROTOCOLS")
else()
  message("linking againt student Bgint lib")
endif()
# add aubdirectories
add_subdirectory(utils)
add_subdirectory(ascon)
add_subdirectory(isap)
add_subdirectory(ecclib)
add_subdirectory(tls)
add_subdirectory(pir)

message(STATUS "---------------------------------------------------------------------")
message(STATUS "Configuration for ${CMAKE_PROJECT_NAME} done.")
message(STATUS "BUILD TYPE:        ${CMAKE_BUILD_TYPE}")
message(STATUS "CC:                ${CMAKE_C_COMPILER}")
message(STATUS "CXX:               ${CMAKE_CXX_COMPILER}")
message(STATUS "")
message(STATUS "Available targets:")
message(STATUS "  all...............Builds all programs.")
message(STATUS "  test.............Runs the test program.")
message(STATUS "  doxygen...........Build doxygen documentation.")
message(STATUS "---------------------------------------------------------------------")
