#!/bin/bash

# PhageForge File Checker
# Checks if all required files exist and have content

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  PhageForge File Content Checker     ${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# Counters
TOTAL_FILES=0
MISSING_FILES=0
EMPTY_FILES=0
OK_FILES=0

# Function to check if a file exists and has content
check_file() {
    local file="$1"
    local description="$2"
    local should_have_code="$3"
    
    ((TOTAL_FILES++))
    
    if [ ! -f "$file" ]; then
        echo -e "${RED}✗ MISSING${NC}: $file ($description)"
        ((MISSING_FILES++))
        return 1
    fi
    
    # Check if file is empty (0 bytes)
    if [ ! -s "$file" ]; then
        echo -e "${RED}✗ EMPTY${NC}: $file ($description)"
        ((EMPTY_FILES++))
        return 1
    fi
    
    # For JSON files, check if they're valid JSON (Phase 2)
    if [[ "$file" == *.json ]]; then
        if command -v jq &> /dev/null; then
            if ! jq empty "$file" 2>/dev/null; then
                echo -e "${RED}✗ INVALID JSON${NC}: $file ($description)"
                ((EMPTY_FILES++))
                return 1
            fi
        fi
    fi
    
    # For TOML files, just check they have content
    if [[ "$file" == *.toml ]]; then
        # Check if it has at least one non-comment line
        if ! grep -v "^#" "$file" | grep -q "."; then
            echo -e "${RED}✗ EMPTY/COMMENTS ONLY${NC}: $file ($description)"
            ((EMPTY_FILES++))
            return 1
        fi
    fi
    
    # For C++ files, check they have actual code (not just comments)
    if [[ "$file" == *.cpp ]] || [[ "$file" == *.hpp ]]; then
        # Check if there's actual code (not just comments/includes)
        if ! grep -v "^[[:space:]]*//" "$file" | grep -v "^[[:space:]]*#" | grep -v "^[[:space:]]*$" | grep -q "."; then
            echo -e "${YELLOW}⚠ WARNING${NC}: $file may only have comments ($description)"
            ((OK_FILES++))
            return 0
        fi
    fi
    
    echo -e "${GREEN}✓ OK${NC}: $file ($description)"
    ((OK_FILES++))
    return 0
}

echo -e "${BLUE}Checking configuration files...${NC}"
check_file "config/amino_acids.toml" "Amino acid properties database" "yes"
echo ""

echo -e "${BLUE}Checking data files...${NC}"
check_file "data/bacteria/e_coli_o157.json" "E. coli strain data" "yes"
check_file "data/bacteria/pseudomonas_aeruginosa.json" "P. aeruginosa strain data" "yes"
check_file "data/genomes/default_phage.json" "Default phage design" "yes"
check_file "data/genomes/lab_strain_phage.json" "Lab strain phage design" "yes"
echo ""

echo -e "${BLUE}Checking documentation...${NC}"
check_file "docs/PHASE1_SPECIFICATION.md" "Phase 1 specification" "yes"
echo ""

echo -e "${BLUE}Checking biology files...${NC}"
check_file "src/biology/AminoAcid.cpp" "Amino acid implementation" "yes"
check_file "src/biology/AminoAcid.hpp" "Amino acid header" "yes"
check_file "src/biology/Bacteria.cpp" "Bacteria implementation" "yes"
check_file "src/biology/Bacteria.hpp" "Bacteria header" "yes"
check_file "src/biology/Codon.cpp" "Codon implementation" "yes"
check_file "src/biology/Codon.hpp" "Codon header" "yes"
check_file "src/biology/Genome.cpp" "Genome implementation" "yes"
check_file "src/biology/Genome.hpp" "Genome header" "yes"
check_file "src/biology/QuorumSensing.cpp" "Quorum sensing implementation" "yes"
check_file "src/biology/QuorumSensing.hpp" "Quorum sensing header" "yes"
check_file "src/biology/Receptor.cpp" "Receptor implementation" "yes"
check_file "src/biology/Receptor.hpp" "Receptor header" "yes"
echo ""

echo -e "${BLUE}Checking core files...${NC}"
check_file "src/core/Constants.hpp" "Physical constants header" "yes"
check_file "src/core/Exceptions.hpp" "Exception classes header" "yes"
check_file "src/core/Types.hpp" "Type definitions header" "yes"
check_file "src/core/Utilities.cpp" "Utility function implementation" "yes"
check_file "src/core/Utilities.hpp" "Utility function header" "yes"
echo ""

echo -e "${BLUE}Checking physics files...${NC}"
check_file "src/physics/BindingAssay.cpp" "Binding assay implementation" "yes"
check_file "src/physics/BindingAssay.hpp" "Binding assay header" "yes"
check_file "src/physics/Electrostatics.cpp" "Electrostatics implementation" "yes"
check_file "src/physics/Electrostatics.hpp" "Electrostatics header" "yes"
check_file "src/physics/VectorMath.cpp" "Vector math implementation" "yes"
check_file "src/physics/VectorMath.hpp" "Vector math header" "yes"
echo ""

echo -e "${BLUE}Checking serialization files...${NC}"
check_file "src/serialization/BinarySerializer.cpp" "Binary serializer implementation" "yes"
check_file "src/serialization/BinarySerializer.hpp" "Binary serializer header" "yes"
check_file "src/serialization/JSONSerializer.cpp" "JSON serializer implementation" "yes"
check_file "src/serialization/JSONSerializer.hpp" "JSON serializer header" "yes"
echo ""

echo -e "${BLUE}Checking source root files...${NC}"
check_file "src/CMakeLists.txt" "Source build configuration" "yes"
check_file "src/main.cpp" "Main test harness" "yes"
echo ""

echo -e "${BLUE}Checking test files...${NC}"
check_file "tests/CMakeLists.txt" "Test build configuration" "yes"
check_file "tests/unit/test_codon.cpp" "Codon unit tests" "yes"
check_file "tests/unit/test_genome.cpp" "Genome unit tests" "yes"
check_file "tests/unit/test_electrostatics.cpp" "Electrostatics unit tests" "yes"
check_file "tests/unit/test_quorum.cpp" "Quorum sensing unit tests" "yes"
check_file "tests/integration/test_binding_pathway.cpp" "Integration tests" "yes"
echo ""

echo -e "${BLUE}Checking root files...${NC}"
check_file ".clang-format" "Code formatting rules" "yes"
check_file ".clang-tidy" "Static analysis rules" "yes"
check_file ".gitignore" "Git ignore patterns" "yes"
check_file "CMakeLists.txt" "Root build configuration" "yes"
check_file "build_and_run.sh" "Build and run script" "yes"
echo ""

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}          Summary                      ${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "${GREEN}OK: $OK_FILES${NC}"
echo -e "${RED}MISSING: $MISSING_FILES${NC}"
echo -e "${RED}EMPTY: $EMPTY_FILES${NC}"
echo -e "${BLUE}Total: $TOTAL_FILES${NC}"
echo ""

if [ $MISSING_FILES -eq 0 ] && [ $EMPTY_FILES -eq 0 ]; then
    echo -e "${GREEN}🎉 ALL FILES ARE PRESENT AND HAVE CONTENT!${NC}"
    echo ""
    echo -e "${BLUE}Phase 1 is 100% complete!${NC}"
    echo -e "  You can now build and run: ${YELLOW}./build_and_run.sh${NC}"
    exit 0
else
    echo -e "${RED}❌ Some files need attention:${NC}"
    if [ $MISSING_FILES -gt 0 ]; then
        echo -e "  ${RED}- $MISSING_FILES files are MISSING${NC}"
    fi
    if [ $EMPTY_FILES -gt 0 ]; then
        echo -e "  ${RED}- $EMPTY_FILES files are EMPTY${NC}"
    fi
    echo ""
    echo -e "${YELLOW}Please create/fill the missing/empty files above.${NC}"
    exit 1
fi
