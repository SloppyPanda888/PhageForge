# Unit tests - GoogleTest is optional for Phase 1
find_package(GTest QUIET)

if(GTest_FOUND)
    enable_testing()
    
    # Codon tests
    add_executable(test_codon
        unit/test_codon.cpp
    )
    target_link_libraries(test_codon PRIVATE phageforge_core ${PLATFORM_LIBS})
    target_include_directories(test_codon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
    add_test(NAME CodonTests COMMAND test_codon)
    
    # Genome tests
    add_executable(test_genome
        unit/test_genome.cpp
    )
    target_link_libraries(test_genome PRIVATE phageforge_core ${PLATFORM_LIBS})
    target_include_directories(test_genome PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
    add_test(NAME GenomeTests COMMAND test_genome)
    
    # Electrostatics tests
    add_executable(test_electrostatics
        unit/test_electrostatics.cpp
    )
    target_link_libraries(test_electrostatics PRIVATE phageforge_core ${PLATFORM_LIBS})
    target_include_directories(test_electrostatics PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
    add_test(NAME ElectrostaticsTests COMMAND test_electrostatics)
    
    # Quorum sensing tests
    add_executable(test_quorum
        unit/test_quorum.cpp
    )
    target_link_libraries(test_quorum PRIVATE phageforge_core ${PLATFORM_LIBS})
    target_include_directories(test_quorum PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
    add_test(NAME QuorumTests COMMAND test_quorum)
    
    # Integration tests
    add_executable(test_binding_pathway
        integration/test_binding_pathway.cpp
    )
    target_link_libraries(test_binding_pathway PRIVATE phageforge_core ${PLATFORM_LIBS})
    target_include_directories(test_binding_pathway PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
    add_test(NAME BindingPathwayTests COMMAND test_binding_pathway)
    
    message(STATUS "GoogleTest found - Unit tests enabled")
else()
    message(STATUS "GoogleTest not found - Unit tests disabled (using stubs)")
    
    # Create stub executables with main() functions
    add_executable(test_codon unit/test_codon.cpp)
    add_executable(test_genome unit/test_genome.cpp)
    add_executable(test_electrostatics unit/test_electrostatics.cpp)
    add_executable(test_quorum unit/test_quorum.cpp)
    add_executable(test_binding_pathway integration/test_binding_pathway.cpp)
    
    # Link with core library
    target_link_libraries(test_codon PRIVATE phageforge_core ${PLATFORM_LIBS})
    target_link_libraries(test_genome PRIVATE phageforge_core ${PLATFORM_LIBS})
    target_link_libraries(test_electrostatics PRIVATE phageforge_core ${PLATFORM_LIBS})
    target_link_libraries(test_quorum PRIVATE phageforge_core ${PLATFORM_LIBS})
    target_link_libraries(test_binding_pathway PRIVATE phageforge_core ${PLATFORM_LIBS})
    
    target_include_directories(test_codon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
    target_include_directories(test_genome PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
    target_include_directories(test_electrostatics PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
    target_include_directories(test_quorum PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
    target_include_directories(test_binding_pathway PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
endif()

# Set output directory for test executables
set_target_properties(test_codon test_genome test_electrostatics test_quorum test_binding_pathway
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
