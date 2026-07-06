# PhageForge - Complete Core Library with GUI

add_library(phageforge_core STATIC
    # Core
    core/Utilities.cpp
    
    # Biology
    biology/Codon.cpp
    biology/AminoAcid.cpp
    biology/Genome.cpp
    biology/Receptor.cpp
    biology/Bacteria.cpp
    biology/QuorumSensing.cpp
    
    # Physics
    physics/VectorMath.cpp
    physics/Electrostatics.cpp
    physics/BindingAssay.cpp
    
    # Serialization
    serialization/JSONSerializer.cpp
    serialization/BinarySerializer.cpp
    
    # GUI
    gui/PhageEditor.cpp
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

# --- GUI Application ---
add_executable(phageforge
    main_gui.cpp
)

target_link_libraries(phageforge
    PRIVATE
    phageforge_core
    ${PLATFORM_LIBS}
    ${OPENGL_LIBRARIES}
    glfw
    GLEW
    imgui
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

set_target_properties(phageforge phageforge_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
