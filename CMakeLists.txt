cmake_minimum_required(VERSION 3.20)

project(PhageForge VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O3 -g")

if(APPLE)
    set(PLATFORM_LIBS "-framework Accelerate -framework OpenGL")
elseif(UNIX)
    find_package(OpenBLAS QUIET)
    if(OpenBLAS_FOUND)
        set(PLATFORM_LIBS ${OpenBLAS_LIBRARIES})
    else()
        set(PLATFORM_LIBS "-lm")
    endif()
    set(PLATFORM_LIBS "${PLATFORM_LIBS} -lGL -lGLU")
endif()

# Phase 4: GUI Libraries
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Boost
find_package(Boost 1.74 REQUIRED COMPONENTS serialization)

# Add ImGui
add_subdirectory(extern/imgui)

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui
    ${Boost_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)

message(STATUS "PhageForge Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Boost: ${Boost_INCLUDE_DIRS}")
message(STATUS "  OpenGL: ${OPENGL_INCLUDE_DIRS}")
