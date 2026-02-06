#!/bin/bash

# Build script for Dilithium benchmark project

set -e  # Exit on error

echo "=========================================="
echo "  Building Dilithium Benchmark"
echo "=========================================="
echo ""

# Check if Dilithium reference code exists
if [ ! -d "dilithium/ref" ]; then
    echo "Error: Dilithium reference implementation not found!"
    echo "Please run ./setup.sh first to download it."
    exit 1
fi

# Create build directory
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

cd build

# Run CMake
echo "Running CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
echo "Building project..."
make -j$(nproc 2>/dev/null || echo 4)

echo ""
echo "=========================================="
echo "  Build completed successfully!"
echo "=========================================="
echo ""
echo "Run the benchmark with:"
echo "  cd build && ./dilithium_benchmark"
echo ""