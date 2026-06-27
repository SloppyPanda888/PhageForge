# Phase 1: Core biological data layer

# Define the static library for core types and biology
add_library(phageforge_core STATIC
    # Core
    core/Utilities.cpp
    
    # Biology
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

# Link dependencies
target_link_libraries(phageforge_core
    PUBLIC
    ${Boost_LIBRARIES}
    ${PLATFORM_LIBS}
)

# The main test harness executable
add_executable(phageforge_test
    main.cpp
)

# Link the core library to the test harness
target_link_libraries(phageforge_test
    PRIVATE
    phageforge_core
    ${PLATFORM_LIBS}
)

# Copy config files to build directory
add_custom_command(
    TARGET phageforge_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../config
        ${CMAKE_CURRENT_BINARY_DIR}/config
    COMMENT "Copying config files to build directory"
)

# Make sure the executable can find the config at runtime
# (This sets the working directory to the build folder)
set_target_properties(phageforge_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
