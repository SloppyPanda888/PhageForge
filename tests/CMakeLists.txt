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
    
    # Physics (stubs for Phase 1 - just empty implementations)
    physics/Electrostatics.cpp
    physics/BindingAssay.cpp
    physics/VectorMath.cpp
    
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

# Make sure the test harness can find the config file at runtime
# This copies the config directory to the build folder
add_custom_command(
    TARGET phageforge_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../config
        ${CMAKE_CURRENT_BINARY_DIR}/config
    COMMENT "Copying config files to build directory"
)
