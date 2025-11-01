cmake_minimum_required(VERSION 3.16)

project(ScaleChordVST3 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Test - just build the core library first
message(STATUS "ScaleChord Core Library - Building...")

# Core library files
add_library(scalechord_core STATIC
    src/ChordAnalyzer.cpp
    src/ChordVoicer.cpp
    src/Envelope.cpp
    src/JazzReharmonizer.cpp
    src/MIDIEffects.cpp
    src/NoteTracker.cpp
    src/ScaleMapper.cpp
    src/VoiceLeading.cpp
)

target_include_directories(scalechord_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

message(STATUS "✓ Core library configured")

# JUCE integration
if(DEFINED JUCE_PATH)
    message(STATUS "JUCE Framework: ${JUCE_PATH}")
    
    add_subdirectory(${JUCE_PATH} JUCE)
    
    # VST3 Plugin target
    juce_add_plugin(ScaleChordPlugin
        COMPANY_NAME "ScaleChord"
        PLUGIN_MANUFACTURER_CODE "Scal"
        PLUGIN_CODE "SCRD"
        FORMATS VST3
        PRODUCT_NAME "ScaleChord"
        IS_MIDI_EFFECT TRUE
        NEEDS_MIDI_INPUT TRUE
        PRODUCES_MIDI_OUTPUT TRUE
    )
    
    # Add plugin sources
    target_sources(ScaleChordPlugin PRIVATE
        src/ChordAnalyzer.cpp
        src/ChordVoicer.cpp
        src/Envelope.cpp
        src/JazzReharmonizer.cpp
        src/MIDIEffects.cpp
        src/NoteTracker.cpp
        src/ScaleMapper.cpp
        src/VoiceLeading.cpp
        juce_plugin/src/PluginProcessor.cpp
        juce_plugin/src/PluginEditor.cpp
    )
    
    target_include_directories(ScaleChordPlugin PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/juce_plugin/include
        /usr/include/gtk-3.0
        /usr/include/pango-1.0
        /usr/include/glib-2.0
        /usr/lib/x86_64-linux-gnu/glib-2.0/include
        /usr/include/harfbuzz
        /usr/include/freetype2
        /usr/include/libpng16
        /usr/include/libmount
        /usr/include/blkid
        /usr/include/fribidi
        /usr/include/cairo
        /usr/include/pixman-1
        /usr/include/gdk-pixbuf-2.0
        /usr/include/x86_64-linux-gnu
        /usr/include/webp
        /usr/include/gio-unix-2.0
        /usr/include/atk-1.0
        /usr/include/at-spi2-atk/2.0
        /usr/include/at-spi-2.0
        /usr/include/dbus-1.0
        /usr/lib/x86_64-linux-gnu/dbus-1.0/include
        /usr/include/webkit2gtk-4.1
    )
    
    # Make core library headers available as system includes
    target_include_directories(ScaleChordPlugin SYSTEM PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    
    target_link_libraries(ScaleChordPlugin PRIVATE
        juce::juce_audio_processors
        juce::juce_core
        juce::juce_events
        juce::juce_gui_basics
        juce::juce_dsp
    )
    
    # Disable GUI extra module to avoid GTK/Pango/HarfBuzz dependencies
    target_compile_definitions(ScaleChordPlugin PRIVATE
        JUCE_DISABLE_NATIVE_HEADERVIEW_DRAGGING=1
    )
    
    message(STATUS "✓ VST3 Plugin configured")
else()
    message(STATUS "⚠ JUCE_PATH not set - VST3 plugin target skipped")
    message(STATUS "   To build VST3 plugin, run: cmake -DJUCE_PATH=/path/to/JUCE .")
endif()

message(STATUS "")
message(STATUS "Build commands:")
message(STATUS "  cmake --build . --config Release")
message(STATUS "  cmake --install . --prefix dist")
message(STATUS "")
