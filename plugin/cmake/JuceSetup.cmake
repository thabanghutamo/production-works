# JuceSetup.cmake - JUCE 7.0+ Integration Module
# This file handles JUCE framework integration for ScaleChord VST3 plugin

# ============================================================================
# JUCE Framework Detection and Integration
# ============================================================================

function(setup_juce)
    message(STATUS "[JUCE] Initializing JUCE integration for ScaleChord UI Framework...")
    
    # Check for JUCE via environment variable first
    if(DEFINED ENV{JUCE_PATH})
        set(JUCE_DIR "$ENV{JUCE_PATH}")
        message(STATUS "[JUCE] Found JUCE_PATH environment variable: ${JUCE_DIR}")
    else()
        # Fall back to system JUCE installation paths
        find_package(JUCE 7.0 QUIET)
        if(JUCE_FOUND)
            message(STATUS "[JUCE] Found system JUCE installation")
        else()
            message(WARNING "[JUCE] JUCE 7.0+ not found. Will use header-only fallback mode.")
            message(STATUS "[JUCE] To use full JUCE integration:")
            message(STATUS "[JUCE]   1. Download JUCE from https://juce.com/download")
            message(STATUS "[JUCE]   2. Set: export JUCE_PATH=/path/to/JUCE")
            message(STATUS "[JUCE]   3. Rebuild with: cmake --build build --target clean && cmake ..")
            return()
        endif()
    endif()
    
    message(STATUS "[JUCE] Setting up JUCE for VST3 and AU plugin targets")
    message(STATUS "[JUCE] Version: 7.0 or later")
    message(STATUS "[JUCE] Target: ScaleChord VST3/AU plugin with UI")
    
endfunction()

# ============================================================================
# Configure JUCE Project Template
# ============================================================================

function(add_scalechord_plugin)
    if(NOT TARGET juce_core)
        message(STATUS "[JUCE] JUCE not fully available, using stub mode")
        return()
    endif()
    
    message(STATUS "[JUCE] Adding ScaleChord plugin target with JUCE support...")
    
    # Add the plugin with JUCE processor wrapper
    add_executable(scalechord_plugin_ui
        src/advanced_demo.cpp
    )
    
    target_link_libraries(scalechord_plugin_ui PRIVATE
        scalechord_core
        juce::juce_core
        juce::juce_gui_basics
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_processors
        juce::juce_audio_utils
    )
    
    # JUCE Audio Plugin Module configuration
    juce_add_vst3(scalechord_plugin_ui
        PRODUCT_NAME "ScaleChord"
        DESCRIPTION "Advanced Scale/Chord MIDI Processor VST3"
        VERSION 1.0.0
        COMPANY_NAME "ScaleChord"
        COMPANY_WEBSITE "https://scalechord.dev"
        COMPANY_EMAIL "info@scalechord.dev"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT TRUE
        IS_MIDI_EFFECT TRUE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        PLUGIN_MANUFACTURER_CODE "SCHL"
        PLUGIN_CODE "SCLE"
    )
    
    message(STATUS "[JUCE] ScaleChord VST3 plugin target configured")
    
endfunction()

# ============================================================================
# Minimum JUCE Mock Headers (when JUCE is not available)
# ============================================================================

function(create_juce_mock_headers)
    message(STATUS "[JUCE] Creating JUCE mock headers for stub mode compilation...")
    
    if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/juce_headers")
        file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/juce_headers")
        
        # Create minimal JUCE::AudioProcessorEditor mock
        file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/juce_headers/juce_audio_processors.h"
            "// Mock JUCE AudioProcessorEditor for stub compilation\n"
            "#pragma once\n"
            "#include <cstdint>\n"
            "namespace juce {\n"
            "  class Component { public: virtual ~Component() {} virtual void resized() {} };\n"
            "  class AudioProcessorEditor : public Component { "
            "  public: AudioProcessorEditor(void* proc) {} "
            "  };\n"
            "}\n"
        )
        
        message(STATUS "[JUCE] Mock headers created in ${CMAKE_CURRENT_BINARY_DIR}/juce_headers")
    endif()
endfunction()

# Call setup when module is loaded
setup_juce()
create_juce_mock_headers()
