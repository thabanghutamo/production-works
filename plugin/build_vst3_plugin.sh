#!/bin/bash

################################################################################
# ScaleChord VST3 Plugin Build Script
# 
# This script builds the ScaleChord VST3 plugin with JUCE framework integration.
# It handles compilation, linking, and packaging.
#
# Usage: ./build_vst3_plugin.sh [options]
# Options:
#   --clean     Clean build directory before building
#   --release   Build Release configuration (default)
#   --debug     Build Debug configuration
#   --install   Install plugin to system location
#   --help      Show this help message
################################################################################

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PLUGIN_DIR="${SCRIPT_DIR}"
JUCE_PATH="${JUCE_PATH:-/tmp/JUCE}"
BUILD_TYPE="Release"
CLEAN_BUILD=0
INSTALL_PLUGIN=0

# Print functions
print_header() {
    echo -e "${BLUE}╔════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║${NC} $1"
    echo -e "${BLUE}╚════════════════════════════════════════════════════════════╝${NC}"
}

print_info() {
    echo -e "${GREEN}✓${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}⚠${NC} $1"
}

print_error() {
    echo -e "${RED}✗${NC} $1"
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --clean)
            CLEAN_BUILD=1
            shift
            ;;
        --release)
            BUILD_TYPE="Release"
            shift
            ;;
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --install)
            INSTALL_PLUGIN=1
            shift
            ;;
        --help)
            cat "$0" | grep -E "^#|Usage|Options"
            exit 0
            ;;
        *)
            print_error "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Check prerequisites
print_header "Checking Prerequisites"

if [[ ! -d "$JUCE_PATH" ]]; then
    print_error "JUCE not found at $JUCE_PATH"
    print_info "Please set JUCE_PATH environment variable or clone JUCE:"
    echo "  git clone https://github.com/juce-framework/JUCE.git /path/to/JUCE"
    exit 1
fi

if ! command -v cmake &> /dev/null; then
    print_error "CMake is not installed"
    exit 1
fi

if ! command -v g++ &> /dev/null; then
    print_error "G++ is not installed"
    exit 1
fi

print_info "CMake version: $(cmake --version | head -1)"
print_info "G++ version: $(g++ --version | head -1)"
print_info "JUCE path: $JUCE_PATH"
print_info "Build type: $BUILD_TYPE"

# Clean if requested
if [[ $CLEAN_BUILD -eq 1 ]]; then
    print_header "Cleaning Previous Build"
    rm -rf "${PLUGIN_DIR}/vst3_build"
    print_info "Cleaned vst3_build directory"
fi

# Create build directory
mkdir -p "${PLUGIN_DIR}/vst3_build"
cd "${PLUGIN_DIR}/vst3_build"

# Configure CMake
print_header "Configuring CMake"

# First try with the fixed CMakeLists.txt
cmake -DJUCE_PATH="$JUCE_PATH" \
       -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
       -DCMAKE_C_COMPILER=/usr/bin/gcc \
       -DCMAKE_CXX_COMPILER=/usr/bin/g++ \
       -G "Unix Makefiles" \
       "${PLUGIN_DIR}" 2>&1 | tee cmake_config.log

# Check if configuration was successful
if [ $? -ne 0 ]; then
    print_error "CMake configuration failed"
    print_info "See cmake_config.log for details"
    exit 1
fi

print_info "CMake configuration successful"

# Build
print_header "Building ScaleChord VST3 Plugin"

make -j$(nproc) 2>&1 | tee build.log

if [ $? -ne 0 ]; then
    print_error "Build failed"
    print_info "See build.log for details"
    exit 1
fi

print_info "Build completed successfully"

# List generated plugin
print_header "Build Output"

# Find VST3 plugin
if [[ "$BUILD_TYPE" == "Debug" ]]; then
    VST3_PLUGIN=$(find . -name "*.vst3" -type d 2>/dev/null | head -1)
else
    VST3_PLUGIN=$(find . -name "*.vst3" -type d 2>/dev/null | head -1)
fi

if [[ -n "$VST3_PLUGIN" ]]; then
    print_info "VST3 Plugin: $VST3_PLUGIN"
    print_info "Plugin size: $(du -sh "$VST3_PLUGIN" | cut -f1)"
else
    print_warning "VST3 plugin not found in expected location"
    print_info "Searching for any generated binaries..."
    find . -name "ScaleChord*" -type f 2>/dev/null | head -10 || true
fi

# Installation (optional)
if [[ $INSTALL_PLUGIN -eq 1 ]]; then
    print_header "Installing Plugin"
    
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        VST3_DEST="$HOME/.vst3"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        VST3_DEST="$HOME/Library/Audio/Plug-Ins/VST3"
    elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
        VST3_DEST="C:\\Program Files\\Common Files\\VST3"
    else
        VST3_DEST="$HOME/.vst3"
    fi
    
    mkdir -p "$VST3_DEST"
    
    if [[ -n "$VST3_PLUGIN" ]]; then
        cp -r "$VST3_PLUGIN" "$VST3_DEST/"
        print_info "Plugin installed to: $VST3_DEST"
    else
        print_warning "Could not find plugin to install"
    fi
fi

# Summary
print_header "Build Summary"
print_info "Build Type: $BUILD_TYPE"
print_info "Build Directory: ${PLUGIN_DIR}/vst3_build"
print_info "Core Library: ✓ Compiled"
print_info "Plugin Wrapper: ✓ Compiled"
print_info "JUCE Integration: ✓ Configured"

echo ""
echo -e "${GREEN}✓ VST3 Plugin Build Complete!${NC}"
echo ""
echo "Next steps:"
echo "  1. Test the plugin: cmake --build vst3_build"
echo "  2. Install the plugin: ./build_vst3_plugin.sh --install"
echo "  3. Load in your DAW"
echo ""
