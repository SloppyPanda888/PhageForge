#!/bin/bash

# PhageForge Build Script
# Usage: ./build.sh [clean|debug|release]

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}    PhageForge Build System v0.1.0      ${NC}"
echo -e "${BLUE}========================================${NC}"

# Check for build type argument
BUILD_TYPE="Release"
if [ "$1" == "debug" ]; then
    BUILD_TYPE="Debug"
elif [ "$1" == "clean" ]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    rm -rf build
    echo -e "${GREEN}✓ Clean complete${NC}"
    exit 0
elif [ "$1" == "help" ]; then
    echo -e "Usage: ./build.sh [command]"
    echo -e ""
    echo -e "Commands:"
    echo -e "  (none)    - Build in Release mode"
    echo -e "  debug     - Build in Debug mode with symbols"
    echo -e "  clean     - Remove build directory"
    echo -e "  help      - Show this help message"
    echo -e ""
    echo -e "Examples:"
    echo -e "  ./build.sh          # Release build"
    echo -e "  ./build.sh debug    # Debug build"
    echo -e "  ./build.sh clean    # Clean everything"
    exit 0
fi

echo -e "${BLUE}Build Type: ${BUILD_TYPE}${NC}"

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure with CMake
echo -e "${BLUE}Configuring CMake...${NC}"
cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build
echo -e "${BLUE}Building PhageForge...${NC}"
make -j$(nproc)

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Build successful!${NC}"
    echo -e ""
    echo -e "${BLUE}Executable location:${NC}"
    echo -e "  ./build/phageforge_test"
    echo -e ""
    echo -e "${BLUE}To run the test harness:${NC}"
    echo -e "  ./build/phageforge_test"
    echo -e ""
    echo -e "${BLUE}To run all tests (if GoogleTest is installed):${NC}"
    echo -e "  cd build && ctest"
else
    echo -e "${RED}✗ Build failed!${NC}"
    exit 1
fi
