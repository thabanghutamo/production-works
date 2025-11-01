cmake_minimum_required(VERSION 3.16)
project(ScaleChordPlugin VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================
# JUCE Integration Configuration (Task 8: UI Framework)
# ============================================================================
option(USE_JUCE "Build with JUCE for full VST3/AU plugin support" ON)
option(JUCE_BUILD_EXTRAS "Build JUCE extras" OFF)

# ScaleChord Core Library (zero external dependencies)
# This library provides the MIDI mapping core independent of any plugin framework

add_library(scalechord_core
    src/ScaleMapper.cpp
    src/ChordVoicer.cpp
    src/Envelope.cpp
    src/NoteTracker.cpp
    src/MIDIEffects.cpp
    src/ChordAnalyzer.cpp
    src/VoiceLeading.cpp
    src/JazzReharmonizer.cpp
    src/PresetManager.cpp
    src/EffectsChain.cpp
    src/PerformanceDashboard.cpp
    src/PluginEditor.cpp
    src/PresetBrowser.cpp
)

target_include_directories(scalechord_core PUBLIC include)

add_executable(scalechord_demo src/demo_main.cpp)
target_link_libraries(scalechord_demo PRIVATE scalechord_core)

add_executable(scalechord_advanced src/advanced_demo.cpp)
target_link_libraries(scalechord_advanced PRIVATE scalechord_core)

enable_testing()
add_executable(test_scale tests/test_scale.cpp)
target_link_libraries(test_scale PRIVATE scalechord_core)
add_test(NAME scale_tests COMMAND test_scale)

add_executable(test_chord_intelligence tests/test_chord_intelligence.cpp)
target_link_libraries(test_chord_intelligence PRIVATE scalechord_core)
add_test(NAME chord_intelligence_tests COMMAND test_chord_intelligence)

add_executable(test_presets tests/test_presets.cpp)
target_link_libraries(test_presets PRIVATE scalechord_core)
add_test(NAME preset_tests COMMAND test_presets)

add_executable(test_effects_chain tests/test_effects_chain.cpp)
target_link_libraries(test_effects_chain PRIVATE scalechord_core)
add_test(NAME effects_chain_tests COMMAND test_effects_chain)

add_executable(test_performance_dashboard tests/test_performance_dashboard.cpp)
target_link_libraries(test_performance_dashboard PRIVATE scalechord_core)
add_test(NAME performance_dashboard_tests COMMAND test_performance_dashboard)

add_executable(scalechord_benchmark tests/benchmark.cpp)
target_link_libraries(scalechord_benchmark PRIVATE scalechord_core)
# Don't add to tests - this is a standalone benchmark, not a unit test

# ============================================================================
# JUCE Integration Module (Task 8: UI Framework)
# ============================================================================
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(JuceSetup)

# Optional: Add JUCE plugin target if JUCE is available
if(USE_JUCE AND TARGET juce_core)
    add_scalechord_plugin()
endif()

# ============================================================================
# Build Summary
# ============================================================================
message(STATUS "")
message(STATUS "ScaleChord Plugin Build Configuration:")
message(STATUS "  Version: 1.0.0")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  JUCE Integration: ${USE_JUCE}")
message(STATUS "  Tests: ENABLED (5 test suites)")
message(STATUS "")
