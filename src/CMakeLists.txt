# PhageForge - Complete Core Library with GUI

add_library(phageforge_core STATIC
    # --- Core Utilities (Phase 1) ---
    core/Utilities.cpp
    
    # --- Biology Layer (Phase 1-2) ---
    biology/Codon.cpp
    biology/AminoAcid.cpp
    biology/Genome.cpp
    biology/Receptor.cpp
    biology/Bacteria.cpp
    biology/QuorumSensing.cpp
    
    # --- Physics Layer (Phase 1-3) ---
    physics/VectorMath.cpp
    physics/Electrostatics.cpp
    physics/BindingAssay.cpp
    
    # --- Serialization (Phase 1) ---
    serialization/JSONSerializer.cpp
    serialization/BinarySerializer.cpp
    
    # --- Phase 4: GUI ---
    gui/PhageEditor.cpp
    gui/BacteriaViewer.cpp
    gui/BindingVisualizer.cpp
)

# Include directories for the library
target_include_directories(phageforge_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Link dependencies
target_link_libraries(phageforge_core
    PUBLIC
    ${Boost_LIBRARIES}
    ${PLATFORM_LIBS}
    ${OPENGL_LIBRARIES}
    glfw
    GLEW
)

# --- Main Application (with GUI) ---
add_executable(phageforge
    main.cpp
    render/OpenGLRenderer.cpp
    render/Shader.cpp
)

target_link_libraries(phageforge
    PRIVATE
    phageforge_core
    ${PLATFORM_LIBS}
    ${OPENGL_LIBRARIES}
    glfw
    GLEW
)

# --- Test Harness (Terminal only) ---
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

set_target_properties(phageforge phageforge_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
