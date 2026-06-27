#!/bin/bash

# PhageForge File Checker - Simplified
# Checks if all required files exist and have content

echo "========================================"
echo "  PhageForge File Content Checker"
echo "========================================"
echo ""

# Counters
TOTAL=0
MISSING=0
EMPTY=0
OK=0

# Function to check a file
check_file() {
    local file="$1"
    local desc="$2"
    
    TOTAL=$((TOTAL + 1))
    
    if [ ! -f "$file" ]; then
        echo "✗ MISSING: $file ($desc)"
        MISSING=$((MISSING + 1))
        return
    fi
    
    if [ ! -s "$file" ]; then
        echo "✗ EMPTY: $file ($desc)"
        EMPTY=$((EMPTY + 1))
        return
    fi
    
    echo "✓ OK: $file ($desc)"
    OK=$((OK + 1))
}

echo "Checking configuration files..."
check_file "config/amino_acids.toml" "Amino acid properties"
echo ""

echo "Checking data files..."
check_file "data/bacteria/e_coli_o157.json" "E. coli strain"
check_file "data/bacteria/pseudomonas_aeruginosa.json" "P. aeruginosa strain"
check_file "data/genomes/default_phage.json" "Default phage"
check_file "data/genomes/lab_strain_phage.json" "Lab phage"
echo ""

echo "Checking biology files..."
check_file "src/biology/AminoAcid.cpp" "Amino acid impl"
check_file "src/biology/AminoAcid.hpp" "Amino acid header"
check_file "src/biology/Bacteria.cpp" "Bacteria impl"
check_file "src/biology/Bacteria.hpp" "Bacteria header"
check_file "src/biology/Codon.cpp" "Codon impl"
check_file "src/biology/Codon.hpp" "Codon header"
check_file "src/biology/Genome.cpp" "Genome impl"
check_file "src/biology/Genome.hpp" "Genome header"
check_file "src/biology/QuorumSensing.cpp" "Quorum impl"
check_file "src/biology/QuorumSensing.hpp" "Quorum header"
check_file "src/biology/Receptor.cpp" "Receptor impl"
check_file "src/biology/Receptor.hpp" "Receptor header"
echo ""

echo "Checking core files..."
check_file "src/core/Constants.hpp" "Constants header"
check_file "src/core/Exceptions.hpp" "Exceptions header"
check_file "src/core/Types.hpp" "Types header"
check_file "src/core/Utilities.cpp" "Utilities impl"
check_file "src/core/Utilities.hpp" "Utilities header"
echo ""

echo "Checking physics files..."
check_file "src/physics/BindingAssay.cpp" "Binding impl"
check_file "src/physics/BindingAssay.hpp" "Binding header"
check_file "src/physics/Electrostatics.cpp" "Electrostatics impl"
check_file "src/physics/Electrostatics.hpp" "Electrostatics header"
check_file "src/physics/VectorMath.cpp" "Vector impl"
check_file "src/physics/VectorMath.hpp" "Vector header"
echo ""

echo "Checking serialization files..."
check_file "src/serialization/BinarySerializer.cpp" "Binary impl"
check_file "src/serialization/BinarySerializer.hpp" "Binary header"
check_file "src/serialization/JSONSerializer.cpp" "JSON impl"
check_file "src/serialization/JSONSerializer.hpp" "JSON header"
echo ""

echo "Checking src root files..."
check_file "src/CMakeLists.txt" "Src CMake"
check_file "src/main.cpp" "Main test harness"
echo ""

echo "Checking test files..."
check_file "tests/CMakeLists.txt" "Tests CMake"
check_file "tests/unit/test_codon.cpp" "Codon tests"
check_file "tests/unit/test_genome.cpp" "Genome tests"
check_file "tests/unit/test_electrostatics.cpp" "Electrostatics tests"
check_file "tests/unit/test_quorum.cpp" "Quorum tests"
check_file "tests/integration/test_binding_pathway.cpp" "Integration tests"
echo ""

echo "Checking root files..."
check_file ".clang-format" "Clang format"
check_file ".clang-tidy" "Clang tidy"
check_file ".gitignore" "Git ignore"
check_file "CMakeLists.txt" "Root CMake"
check_file "build_and_run.sh" "Build script"
echo ""

echo "========================================"
echo "  Summary"
echo "========================================"
echo "✓ OK: $OK"
echo "✗ MISSING: $MISSING"
echo "✗ EMPTY: $EMPTY"
echo "Total: $TOTAL"
echo ""

if [ $MISSING -eq 0 ] && [ $EMPTY -eq 0 ]; then
    echo "🎉 ALL FILES ARE PRESENT AND HAVE CONTENT!"
    echo ""
    echo "Phase 1 is 100% complete!"
    echo "You can now build and run: ./build_and_run.sh"
else
    echo "❌ Some files need attention:"
    if [ $MISSING -gt 0 ]; then
        echo "  - $MISSING files are MISSING"
    fi
    if [ $EMPTY -gt 0 ]; then
        echo "  - $EMPTY files are EMPTY"
    fi
    echo ""
    echo "Please create/fill the missing/empty files above."
fi
