#pragma once

#include <JuceHeader.h>
#include <memory>
#include <array>

// Include ScaleChord core classes
#include "ScaleMapper.h"
#include "ChordVoicer.h"
#include "VoiceLeading.h"
#include "ChordAnalyzer.h"
#include "JazzReharmonizer.h"
#include "NoteTracker.h"
#include "MIDIEffects.h"

/**
 * ScaleChordPluginAudioProcessor
 *
 * Main audio processor for the ScaleChord VST3 plugin.
 * Integrates the core ScaleChord library with JUCE audio processing pipeline.
 *
 * Key responsibilities:
 * - Process incoming MIDI
 * - Generate intelligent chord voicings
 * - Apply voice leading and effects
 * - Manage plugin state and parameters
 */
class ScaleChordPluginAudioProcessor : public juce::AudioProcessor,
                                       public juce::AudioProcessorValueTreeState::Listener
{
public:
    ScaleChordPluginAudioProcessor();
    ~ScaleChordPluginAudioProcessor() override;

    // ========================================================================
    // JUCE Audio Processing Interface
    // ========================================================================

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    // ========================================================================
    // JUCE Editor Interface
    // ========================================================================

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    // ========================================================================
    // JUCE Plugin Information
    // ========================================================================

    const juce::String getName() const override { return "ScaleChord"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    bool isMidiEffect() const override { return true; }
    double getTailLengthSeconds() const override { return 0.0; }

    // ========================================================================
    // JUCE Program/Preset Management
    // ========================================================================

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return "Default"; }
    void changeProgramName(int, const juce::String&) override {}

    // ========================================================================
    // JUCE State Serialization
    // ========================================================================

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // ========================================================================
    // ScaleChord Core Integration
    // ========================================================================

    /**
     * Process MIDI note on event
     * Generates intelligent voicing based on current scale and settings
     */
    void processMidiNoteOn(int noteNumber, int velocity);

    /**
     * Process MIDI note off event
     * Handles voice release and note tracking
     */
    void processMidiNoteOff(int noteNumber, int velocity);

    /**
     * Apply intelligent voice leading
     * Reduces jumps between voicings for smooth playback
     */
    void applyVoiceLeading(juce::MidiBuffer& outputMidi);

    /**
     * Get current scale selection
     * Returns active scale for chord generation
     */
    const std::string& getCurrentScale() const { return currentScale; }

    /**
     * Set scale for chord generation
     * Triggers re-voicing of active notes
     */
    void setScale(const std::string& newScale);

    /**
     * Get current voice leading mode
     */
    int getVoiceLeadingMode() const { return voiceLeadingMode; }

    /**
     * Set voice leading mode
     * 0 = Disabled, 1 = Smooth, 2 = Optimal, 3 = Jazz
     */
    void setVoiceLeadingMode(int mode);

    /**
     * Enable/disable MIDI effects
     */
    void setMidiEffectsEnabled(bool enabled) { midiEffectsEnabled = enabled; }

    /**
     * Enable/disable jazz reharmonization
     */
    void setJazzReharmonizationEnabled(bool enabled) { jazzReharmonizationEnabled = enabled; }

    // ========================================================================
    // Parameter Management
    // ========================================================================

    juce::AudioProcessorValueTreeState& getValueTreeState() { return parameters; }
    const juce::AudioProcessorValueTreeState& getValueTreeState() const { return parameters; }

    // ========================================================================
    // Value Tree State Listener
    // ========================================================================

    void parameterChanged(const juce::String& parameterID, float newValue) override;

private:
    // ========================================================================
    // ScaleChord Core Objects
    // ========================================================================

    std::unique_ptr<scalechord::ScaleMapper> scaleMapper;
    std::unique_ptr<scalechord::ChordVoicer> chordVoicer;
    std::unique_ptr<VoiceLeading> voiceLeading;
    std::unique_ptr<ChordAnalyzer> chordAnalyzer;
    std::unique_ptr<JazzReharmonizer> jazzReharmonizer;
    std::unique_ptr<scalechord::NoteTracker> noteTracker;
    // MIDIEffects will be initialized dynamically
    void* midiEffects = nullptr;

    // ========================================================================
    // Plugin State
    // ========================================================================

    juce::AudioProcessorValueTreeState parameters;

    // Current settings
    std::string currentScale = "Major";
    int voiceLeadingMode = 1;  // 0=Off, 1=Smooth, 2=Optimal, 3=Jazz
    bool midiEffectsEnabled = true;
    bool jazzReharmonizationEnabled = true;

    // Active notes tracking
    std::array<int, 16> activeNotes{};  // Max 16 MIDI channels
    std::array<int, 128> noteVoicings{};  // Voicing for each MIDI note

    // Processing state
    double sampleRate = 44100.0;
    int bufferSize = 256;

    // ========================================================================
    // Helper Methods
    // ========================================================================

    /**
     * Create and configure plugin parameters
     */
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    /**
     * Initialize all ScaleChord core modules
     */
    void initializeCoreModules();

    /**
     * Handle scale change with re-voicing
     */
    void onScaleChanged();

    /**
     * Generate voicing for a given root note
     */
    std::vector<int> generateVoicing(int rootNote);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScaleChordPluginAudioProcessor)
};
