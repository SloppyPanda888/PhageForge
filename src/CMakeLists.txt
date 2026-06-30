# Phase 2: Core biological data layer with Quorum Sensing

add_library(phageforge_core STATIC
    # Core
    core/Utilities.cpp
    
    # Biology (Phase 2: Full implementation)
    biology/Codon.cpp
    biology/AminoAcid.cpp
    biology/Genome.cpp
    biology/QuorumSensing.cpp
    biology/Bacteria.cpp
    biology/Receptor.cpp
    
    # Physics (stubs for Phase 1)
    physics/VectorMath.cpp
    physics/Electrostatics.cpp
    physics/BindingAssay.cpp
    
    # Serialization
    serialization/JSONSerializer.cpp
    serialization/BinarySerializer.cpp
)

# Include directories for the library
target_include_directories(phageforge_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Link dependencies (Boost is now required!)
target_link_libraries(phageforge_core
    PUBLIC
    ${Boost_LIBRARIES}
    ${PLATFORM_LIBS}
)

# The main test harness executable
add_executable(phageforge_test
    main.cpp
)

target_link_libraries(phageforge_test
    PRIVATE
    phageforge_core
    ${PLATFORM_LIBS}
)

# Copy config files
add_custom_command(
    TARGET phageforge_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../config
        ${CMAKE_CURRENT_BINARY_DIR}/config
    COMMENT "Copying config files to build directory"
)

set_target_properties(phageforge_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
