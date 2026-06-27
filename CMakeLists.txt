cmake_minimum_required(VERSION 3.20)

# PROJECT() MUST COME IMMEDIATELY AFTER CMAKE_MINIMUM_REQUIRED()
project(PhageForge VERSION 0.1.0 LANGUAGES CXX)

# Now set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable warnings and optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O3 -g")

# Detect platform and set libraries
if(APPLE)
    set(PLATFORM_LIBS "-framework Accelerate")
elseif(UNIX)
    # Try to find OpenBLAS, but don't require it
    find_package(OpenBLAS QUIET)
    if(OpenBLAS_FOUND)
        set(PLATFORM_LIBS ${OpenBLAS_LIBRARIES})
        message(STATUS "OpenBLAS found")
    else()
        # Use standard math library if OpenBLAS not found
        set(PLATFORM_LIBS "-lm")
        message(STATUS "OpenBLAS not found, using standard math library")
    endif()
endif()

# Main source subdirectories
add_subdirectory(src)
add_subdirectory(tests)

# Enable testing
enable_testing()

# Global include directories for all targets
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Find Boost (optional for Phase 1 - will be needed for Phase 2)
find_package(Boost 1.75 QUIET COMPONENTS serialization)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "Boost found: ${Boost_INCLUDE_DIRS}")
else()
    message(STATUS "Boost not found - Phase 2 will need it")
endif()

# Print configuration
message(STATUS "PhageForge Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}")
