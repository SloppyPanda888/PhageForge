# PhageForge - Complete Core Library (Phases 1-3)

add_library(phageforge_core STATIC
    # --- Core Utilities (Phase 1) ---
    core/Utilities.cpp
    
    # --- Biology Layer (Phase 1-2) ---
    biology/Codon.cpp          # Phase 1: DNA codon translation
    biology/AminoAcid.cpp      # Phase 1: Amino acid properties
    biology/Genome.cpp         # Phase 1: Genome manipulation
    biology/Receptor.cpp       # Phase 1: Receptor stub (Phase 2: full impl)
    biology/Bacteria.cpp       # Phase 2: Bacterial dynamics
    biology/QuorumSensing.cpp  # Phase 2: Quorum sensing with Boost::odeint
    
    # --- Physics Layer (Phase 1-3) ---
    physics/VectorMath.cpp     # Phase 1: 3D math utilities
    physics/Electrostatics.cpp # Phase 3: Debye-Hückel solver (NOW FULL!)
    physics/BindingAssay.cpp   # Phase 3: Binding scoring (NOW FULL!)
    
    # --- Serialization (Phase 1) ---
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

# --- Main Test Harness ---
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
