cmake_minimum_required(VERSION 3.20)
project(PhageForge VERSION 0.1.0 LANGUAGES CXX)

# C++20 standard with optimizations for scientific computing
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable expensive optimizations but keep debug symbols for profiling
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -march=native -O3 -flto -g")

# Detect platform
if(APPLE)
    set(PLATFORM_LIBS "-framework Accelerate")
elseif(UNIX)
    set(PLATFORM_LIBS "-lopenblas -llapack")
endif()

# External dependencies (header-only for Phase 1)
add_subdirectory(extern/eigen-3.4.0)
add_subdirectory(extern/tomlplusplus-3.4.0)
add_subdirectory(extern/fmt-10.0.0)

# Main source subdirectories
add_subdirectory(src)
add_subdirectory(tests)

# Enable testing
enable_testing()

# Global include directories for all targets
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/eigen-3.4.0
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/tomlplusplus-3.4.0/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/fmt-10.0.0/include
)

# Find Boost for ODE solving (will be used in Phase 2)
find_package(Boost 1.75 REQUIRED COMPONENTS serialization)
include_directories(${Boost_INCLUDE_DIRS})
