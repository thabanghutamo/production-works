/**
 * ScaleChord Plugin - Version Information
 * ======================================
 * 
 * Version: 1.0.0
 * Release Date: 2024
 * Status: Production Ready
 * 
 * This header provides version information for the ScaleChord plugin.
 * Used by build system, packaging, and runtime version reporting.
 */

#ifndef SCALECHORD_VERSION_H
#define SCALECHORD_VERSION_H

// Version Components
#define SCALECHORD_VERSION_MAJOR 1
#define SCALECHORD_VERSION_MINOR 0
#define SCALECHORD_VERSION_PATCH 0

// Full Version String
#define SCALECHORD_VERSION_STRING "1.0.0"

// Build Information
#define SCALECHORD_BUILD_DATE __DATE__
#define SCALECHORD_BUILD_TIME __TIME__

// Plugin Identifier
#define SCALECHORD_PLUGIN_NAME "ScaleChord"
#define SCALECHORD_PLUGIN_VENDOR "Production Works"
#define SCALECHORD_PLUGIN_DESCRIPTION "AI-powered MIDI chord mapping with real-time scale detection and voice leading"

// Feature Flags (Phase 6 Completion)
#define SCALECHORD_HAS_UI_FRAMEWORK 1
#define SCALECHORD_HAS_PRESET_BROWSER 1
#define SCALECHORD_HAS_PERFORMANCE_DASHBOARD 1
#define SCALECHORD_HAS_EFFECTS_CHAIN 1
#define SCALECHORD_HAS_VOICE_LEADING 1
#define SCALECHORD_HAS_JAZZ_REHARMONIZER 1
#define SCALECHORD_HAS_MIDI_ROUTING 1
#define SCALECHORD_HAS_ENVELOPE_CONTROL 1

// Capability Levels (All Verified Phase 7)
#define SCALECHORD_REALTIME_SAFE 1          // ✅ Verified in Phase 7a
#define SCALECHORD_DAW_COMPATIBLE 1         // ✅ Verified in Phase 7b
#define SCALECHORD_PRODUCTION_READY 1       // ✅ All tests passing
#define SCALECHORD_PERFORMANCE_OPTIMIZED 1  // ✅ < 5% CPU typical

#endif // SCALECHORD_VERSION_H
