#!/bin/bash

# PhageForge Build and Run Script

echo "========================================"
echo "  PhageForge Phase 1 Build & Run"
echo "========================================"

# Clean previous build
echo "Cleaning build directory..."
rm -rf build

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
echo "Building..."
make -j$(nproc)

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo "✓ Build successful!"
    echo ""
    echo "Running test harness..."
    echo "========================================"
    
    # Run the executable from the build directory
    ./phageforge_test
    
    echo ""
    echo "========================================"
    echo "✓ Phase 1 Complete!"
else
    echo "✗ Build failed!"
    exit 1
fi

cd ..
