cmake_minimum_required(VERSION 3.20)

project(PhageForge VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O3 -g")

if(APPLE)
    set(PLATFORM_LIBS "-framework Accelerate")
elseif(UNIX)
    find_package(OpenBLAS QUIET)
    if(OpenBLAS_FOUND)
        set(PLATFORM_LIBS ${OpenBLAS_LIBRARIES})
    else()
        set(PLATFORM_LIBS "-lm")
    endif()
endif()

# Phase 2: Boost is now REQUIRED - but use 1.74 if you have it
# Changed from 1.75 to 1.74 to match your system
find_package(Boost 1.74 REQUIRED COMPONENTS serialization)
include_directories(${Boost_INCLUDE_DIRS})

# Main source subdirectories
add_subdirectory(src)
add_subdirectory(tests)

# Enable testing
enable_testing()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

message(STATUS "PhageForge Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Boost: ${Boost_INCLUDE_DIRS}")
