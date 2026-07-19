cmake_minimum_required(VERSION 3.10)

project(PhageForge VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

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

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

find_package(Boost 1.74 REQUIRED COMPONENTS serialization)

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends
    ${Boost_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
)

message(STATUS "PhageForge Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Boost: ${Boost_INCLUDE_DIRS}")
message(STATUS "  OpenGL: ${OPENGL_INCLUDE_DIRS}")
