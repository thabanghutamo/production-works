#!/bin/bash
# setup_juce.sh - Quick JUCE 7.0+ Installation and Integration Script
# Usage: ./setup_juce.sh [--path /custom/juce/path]

set -e

# Default JUCE installation path
JUCE_INSTALL_PATH="${1:-.}"
JUCE_VERSION="7.0.0"
JUCE_REPO="https://github.com/juce-framework/JUCE.git"

echo "╔════════════════════════════════════════════════════════════════╗"
echo "║          ScaleChord JUCE Integration Setup Script              ║"
echo "║                 JUCE Version: ${JUCE_VERSION}                     ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

# Check for required tools
check_requirements() {
    echo "✓ Checking requirements..."
    
    if ! command -v git &> /dev/null; then
        echo "✗ ERROR: git not found. Please install git first."
        exit 1
    fi
    
    if ! command -v cmake &> /dev/null; then
        echo "✗ ERROR: cmake not found. Please install cmake first."
        exit 1
    fi
    
    echo "✓ All requirements met"
    echo ""
}

# Download and setup JUCE
setup_juce() {
    echo "➤ Downloading JUCE ${JUCE_VERSION}..."
    
    if [ -d "$JUCE_INSTALL_PATH/JUCE" ]; then
        echo "  JUCE already exists at $JUCE_INSTALL_PATH/JUCE"
        read -p "  Overwrite? (y/N) " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            rm -rf "$JUCE_INSTALL_PATH/JUCE"
        else
            echo "  Using existing JUCE installation"
            JUCE_INSTALL_PATH="$JUCE_INSTALL_PATH/JUCE"
            return
        fi
    fi
    
    cd "$JUCE_INSTALL_PATH"
    git clone "$JUCE_REPO" JUCE
    cd JUCE
    git checkout ${JUCE_VERSION} 2>/dev/null || git checkout main
    cd - > /dev/null
    
    echo "✓ JUCE downloaded to $JUCE_INSTALL_PATH/JUCE"
    JUCE_INSTALL_PATH="$JUCE_INSTALL_PATH/JUCE"
}

# Configure environment
configure_environment() {
    echo ""
    echo "➤ Configuring environment..."
    
    export JUCE_PATH="$(cd "$JUCE_INSTALL_PATH" && pwd)"
    echo "  JUCE_PATH: $JUCE_PATH"
    
    # Add to .bashrc if not already there
    if ! grep -q "export JUCE_PATH=" ~/.bashrc; then
        echo "  Adding JUCE_PATH to ~/.bashrc"
        echo "" >> ~/.bashrc
        echo "# ScaleChord JUCE Path (added by setup_juce.sh)" >> ~/.bashrc
        echo "export JUCE_PATH='$JUCE_PATH'" >> ~/.bashrc
    fi
    
    echo "✓ Environment configured"
}

# Rebuild ScaleChord plugin
rebuild_plugin() {
    echo ""
    echo "➤ Rebuilding ScaleChord plugin with JUCE support..."
    
    PLUGIN_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)/plugin"
    
    if [ ! -d "$PLUGIN_PATH" ]; then
        echo "✗ ERROR: Plugin directory not found at $PLUGIN_PATH"
        exit 1
    fi
    
    cd "$PLUGIN_PATH"
    
    if [ -d "build" ]; then
        echo "  Cleaning old build..."
        rm -rf build
    fi
    
    mkdir build
    cd build
    
    echo "  Running cmake with JUCE_PATH=$JUCE_PATH..."
    cmake .. -DUSE_JUCE=ON 2>&1 | grep -v "^--" | tail -20
    
    echo "  Building..."
    cmake --build . --parallel $(nproc) 2>&1 | tail -20
    
    echo "✓ Build complete"
}

# Verify installation
verify_installation() {
    echo ""
    echo "➤ Verifying installation..."
    
    PLUGIN_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)/plugin"
    cd "$PLUGIN_PATH/build"
    
    if ctest --output-on-failure 2>&1 | grep -q "100% tests passed"; then
        echo "✓ All tests passing"
    else
        echo "✗ WARNING: Some tests may have failed. Check above for details."
    fi
    
    echo ""
}

# Main execution
main() {
    check_requirements
    setup_juce
    configure_environment
    rebuild_plugin
    verify_installation
    
    echo "╔════════════════════════════════════════════════════════════════╗"
    echo "║         ✓ JUCE Integration Setup Complete!                    ║"
    echo "║                                                                ║"
    echo "║  Next Steps:                                                   ║"
    echo "║  1. Reload shell: source ~/.bashrc                             ║"
    echo "║  2. Verify: echo \$JUCE_PATH                                    ║"
    echo "║  3. Continue to Phase 2: Editor Implementation                 ║"
    echo "║                                                                ║"
    echo "║  JUCE Path: $JUCE_PATH"
    echo "╚════════════════════════════════════════════════════════════════╝"
}

main "$@"
