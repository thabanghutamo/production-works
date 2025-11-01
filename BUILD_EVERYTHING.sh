#!/bin/bash

set -e

echo "╔════════════════════════════════════════════════════════════════╗"
echo "║     ScaleChord v0.3 - Complete Build & Test Suite              ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

# Navigate to plugin directory
cd /workspaces/production-works/plugin

# Create build directory
echo "[1/5] Creating build directory..."
mkdir -p build
cd build

# Configure with CMake
echo "[2/5] Configuring CMake..."
cmake .. > /dev/null 2>&1

# Build all targets
echo "[3/5] Building all targets..."
cmake --build . -j4 > /dev/null 2>&1

# Run unit tests
echo "[4/5] Running unit tests..."
ctest -V --output-on-failure > /dev/null 2>&1

# Run benchmarks
echo "[5/5] Running performance benchmarks..."
./scalechord_benchmark

echo ""
echo "╔════════════════════════════════════════════════════════════════╗"
echo "║                    BUILD COMPLETE ✅                           ║"
echo "╠════════════════════════════════════════════════════════════════╣"
echo "║ Executables:                                                   ║"
echo "║   ./scalechord_demo              - Basic demo                  ║"
echo "║   ./scalechord_advanced          - Advanced features showcase  ║"
echo "║   ./scalechord_benchmark         - Performance benchmark       ║"
echo "║   ./test_scale                   - Unit tests (scales)         ║"
echo "║   ./test_chord_intelligence      - Unit tests (Task 3)         ║"
echo "║                                                                ║"
echo "║ Library:                                                       ║"
echo "║   ../libscalechord_core.a        - Core library                ║"
echo "╚════════════════════════════════════════════════════════════════╝"
