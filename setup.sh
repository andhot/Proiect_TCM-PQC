#!/bin/bash

# Script to download and setup CRYSTALS-Dilithium reference implementation
# This script will download the official Dilithium code from the CRYSTALS team

echo "=========================================="
echo "  Dilithium Reference Implementation Setup"
echo "=========================================="
echo ""

# Check if git is installed
if ! command -v git &> /dev/null; then
    echo "Error: git is not installed. Please install git first."
    exit 1
fi

# Create dilithium directory if it doesn't exist
if [ ! -d "dilithium" ]; then
    echo "Downloading CRYSTALS-Dilithium reference implementation..."
    git clone https://github.com/pq-crystals/dilithium.git
    
    if [ $? -ne 0 ]; then
        echo "Error: Failed to clone Dilithium repository"
        exit 1
    fi
    
    echo "✓ Dilithium downloaded successfully"
else
    echo "✓ Dilithium directory already exists"
fi

echo ""
echo "Checking Dilithium files..."

# Check if reference implementation exists
if [ ! -d "dilithium/ref" ]; then
    echo "Error: dilithium/ref directory not found"
    echo "Please check that the repository was cloned correctly"
    exit 1
fi

# List key files
echo "✓ Found reference implementation in dilithium/ref/"
echo ""
echo "Key files:"
ls -lh dilithium/ref/*.{c,h} 2>/dev/null | awk '{print "  " $9 " (" $5 ")"}'

echo ""
echo "=========================================="
echo "  Setup completed successfully!"
echo "=========================================="
echo ""
echo "Next steps:"
echo "  1. Make sure you have CMake (>= 3.15) and OpenSSL installed"
echo "  2. Create build directory: mkdir build && cd build"
echo "  3. Run CMake: cmake .."
echo "  4. Build: make"
echo "  5. Run benchmark: ./dilithium_benchmark"
echo ""