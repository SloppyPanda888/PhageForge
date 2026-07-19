# PhageForge - Core Library + GUI + 3D Renderer

# --- Core Library ---
add_library(phageforge_core STATIC
    core/Utilities.cpp
    biology/Codon.cpp
    biology/AminoAcid.cpp
    biology/Genome.cpp
    biology/Receptor.cpp
    biology/Bacteria.cpp
    biology/QuorumSensing.cpp
    physics/VectorMath.cpp
    physics/Electrostatics.cpp
    physics/BindingAssay.cpp
    serialization/JSONSerializer.cpp
    serialization/BinarySerializer.cpp
)

target_include_directories(phageforge_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(phageforge_core
    PUBLIC
    ${Boost_LIBRARIES}
    ${PLATFORM_LIBS}
)

# --- GUI Application with 3D Renderer ---
add_executable(phageforge
    main_gui.cpp
    gui/PhageEditor.cpp
    render/OpenGLRenderer.cpp
    # ImGui source files
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(phageforge PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/imgui/backends
)

target_link_libraries(phageforge
    PRIVATE
    phageforge_core
    ${PLATFORM_LIBS}
    ${OPENGL_LIBRARIES}
    glfw
    GLEW
)

# --- CLI Test Harness ---
add_executable(phageforge_test
    main_cli.cpp
)

target_link_libraries(phageforge_test
    PRIVATE
    phageforge_core
    ${PLATFORM_LIBS}
)

# Copy config files
add_custom_command(
    TARGET phageforge POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../config
        ${CMAKE_CURRENT_BINARY_DIR}/config
    COMMENT "Copying config files to build directory"
)

add_custom_command(
    TARGET phageforge_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../config
        ${CMAKE_CURRENT_BINARY_DIR}/config
    COMMENT "Copying config files to build directory"
)

set_target_properties(phageforge phageforge_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

message(STATUS "Building: phageforge (GUI+3D) and phageforge_test (CLI)")
