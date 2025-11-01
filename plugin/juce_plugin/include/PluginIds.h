#pragma once

#ifndef PLUGIN_IDS_H
#define PLUGIN_IDS_H

/*
    ScaleChord VST3 Plugin - Plugin Configuration IDs
    
    This file contains all JUCE plugin configuration macros for the ScaleChord
    VST3 plugin. It defines plugin metadata, capabilities, and behavior.
*/

// ============================================================================
// PLUGIN METADATA
// ============================================================================

#define JucePlugin_Name                "ScaleChord"
#define JucePlugin_Desc                "Intelligent chord generator and voice leading tool for musicians"
#define JucePlugin_Manufacturer        "ScaleChord"
#define JucePlugin_ManufacturerWebsite "https://scalechord.audio"
#define JucePlugin_ManufacturerEmail   "support@scalechord.audio"
#define JucePlugin_Version             "1.0.0"
#define JucePlugin_VersionStr          "1.0.0"

// ============================================================================
// PLUGIN IDENTIFIERS
// ============================================================================

// Unique plugin code - must be exactly 4 characters
#define JucePlugin_PluginCode          'SCRD'

// Format-specific IDs
#define JucePlugin_AUExportPrefix      ScaleChordAU
#define JucePlugin_AUExportPrefixQuoted "ScaleChordAU"
#define JucePlugin_AAXIdentifier       "com.scalechord.scalechord"

// ============================================================================
// PLUGIN CAPABILITIES
// ============================================================================

// Synth vs MIDI effect
#define JucePlugin_IsSynth             0  // 0 = MIDI effect, 1 = synth
#define JucePlugin_IsMidiEffect        1  // Processes MIDI
#define JucePlugin_WantsMidiInput      1  // Accepts MIDI input
#define JucePlugin_ProducesMidiOutput  1  // Produces MIDI output

// Audio processing
#define JucePlugin_ProducesAnalysisSideChainOutput 0
#define JucePlugin_SilenceInProducesSilenceOut     1  // No output when silent

// ============================================================================
// PLUGIN BEHAVIOR
// ============================================================================

// Latency and tail
#define JucePlugin_TailLengthSeconds   0.0
#define JucePlugin_LatencyMs           0
#define JucePlugin_EditorRequiresKeyboardFocus 0

// Channel configuration
#define JucePlugin_MaxNumInputChannels  2
#define JucePlugin_MaxNumOutputChannels 2

// ============================================================================
// BUILD FORMAT SELECTION
// ============================================================================

// VST3 format enabled
#define JucePlugin_VSTUniqueID         JucePlugin_PluginCode
#define JUCE_VST3_CAN_REPLACE_VST2     0

// Format availability
#define JucePlugin_Build_VST           0  // VST2 disabled
#define JucePlugin_Build_VST3          1  // VST3 enabled
#define JucePlugin_Build_AU            1  // AU enabled (macOS)
#define JucePlugin_Build_AUv3          0  // AUv3 disabled (future)
#define JucePlugin_Build_AAX           0  // AAX disabled (future)
#define JucePlugin_Build_Standalone    0  // Standalone disabled (use standalone executable)
#define JucePlugin_Build_UnityPlugin   0  // Unity disabled

// ============================================================================
// URL AND LICENSING
// ============================================================================

#define SCALECHORD_LICENSE_URL         "https://scalechord.audio/license"
#define SCALECHORD_PRIVACY_URL         "https://scalechord.audio/privacy"
#define SCALECHORD_SUPPORT_URL         "https://scalechord.audio/support"
#define SCALECHORD_DOCUMENTATION_URL   "https://scalechord.audio/docs"

// ============================================================================
// VERSION INFORMATION
// ============================================================================

#define SCALECHORD_MAJOR_VERSION       1
#define SCALECHORD_MINOR_VERSION       0
#define SCALECHORD_PATCH_VERSION       0

// Build number (auto-incremented)
#define SCALECHORD_BUILD_NUMBER        1

// ============================================================================
// FEATURE FLAGS
// ============================================================================

// Enable/disable features
#define SCALECHORD_ENABLE_MIDI_LEARNING      1  // Allow MIDI learning
#define SCALECHORD_ENABLE_PRESET_SAVE        1  // Allow preset saving
#define SCALECHORD_ENABLE_SIDECHAIN          0  // No sidechain (future)
#define SCALECHORD_ENABLE_AUTOMATION         1  // Full automation support
#define SCALECHORD_ENABLE_MIDI_CC_MAPPING    1  // MIDI CC mapping

// ============================================================================
// PERFORMANCE SETTINGS
// ============================================================================

#define SCALECHORD_DEFAULT_BUFFER_SIZE       256
#define SCALECHORD_MAX_POLYPHONY             16
#define SCALECHORD_VOICE_LEADING_THRESHOLD   0.5f
#define SCALECHORD_ANALYSIS_SMOOTHING        0.3f

#endif  // PLUGIN_IDS_H
