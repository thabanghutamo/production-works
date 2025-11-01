#pragma once

#if defined(JUCE_MODULE_AVAILABLE_juce_audio_processors)
#include <juce_audio_processors/juce_audio_processors.h>
#else
#error "This module requires JUCE. Ensure JUCE is properly integrated before building."
#endif

#include "../include/ScaleMapper.h"
#include "../include/ChordVoicer.h"
#include "../include/Envelope.h"
#include "../include/NoteTracker.h"
#include "../include/MIDIEffects.h"
#include "../include/ChordAnalyzer.h"
#include "../include/VoiceLeading.h"
#include "../include/JazzReharmonizer.h"
#include "../include/PresetManager.h"
#include "../include/PerformanceDashboard.h"

namespace scalechord {

/**
 * @class PluginProcessor
 * @brief Complete VST3 AudioProcessor integrating all ScaleChord core modules
 * 
 * Features:
 * - Full parameter automation (12+ parameters)
 * - Polyphonic MIDI handling (16 voices)
 * - Real-time preset switching
 * - Advanced voice leading
 * - Jazz reharmonization
 * - Comprehensive MIDI effects
 * - Real-time performance monitoring
 * - Full APVTS support for automation
 */
class PluginProcessor : public juce::AudioProcessor
{
public:
    // Constructor & Destructor
    PluginProcessor();
    ~PluginProcessor() override;

    // AudioProcessor overrides
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    void processBlockBypassed(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    // Parameter management
    int getNumParameters() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float newValue) override;
    const juce::String getParameterName(int index) const override;
    const juce::String getParameterText(int index) const override;

    // Plugin information
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return "ScaleChord"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    bool isMidiEffect() const override { return true; }
    double getTailLengthSeconds() const override { return 0.0; }

    // APVTS and Editor access
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts_; }
    PerformanceDashboard& getDashboard() { return dashboard_; }

    // Program/Preset management
    int getNumPrograms() const override;
    int getCurrentProgram() const override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) const override;
    void changeProgramName(int index, const juce::String& newName) override;

    // State management
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Custom preset methods
    void loadPreset(const Preset& preset);
    void saveCurrentAsPreset(const std::string& name, const std::string& category);
    Preset getCurrentPreset() const;

    // Parameter getters (for UI binding)
    int getRootNote() const { return rootNote_; }
    int getScaleType() const { return scaleType_; }
    int getVoicingType() const { return voicingType_; }
    float getAttackMs() const { return attackMs_; }
    float getDecayMs() const { return decayMs_; }
    float getSustainLevel() const { return sustainLevel_; }
    float getReleaseMs() const { return releaseMs_; }
    bool isLegatoEnabled() const { return legatoEnabled_; }
    bool isChordMemoryEnabled() const { return chordMemoryEnabled_; }
    int getMidiInputChannel() const { return midiInputChannel_; }
    int getMidiOutputChannel() const { return midiOutputChannel_; }
    float getHumanizationAmount() const { return humanizationAmount_; }

    // Parameter setters (for automation)
    void setRootNote(int note);
    void setScaleType(int scaleIdx);
    void setVoicingType(int voicingIdx);
    void setAttackMs(float ms);
    void setDecayMs(float ms);
    void setSustainLevel(float level);
    void setReleaseMs(float ms);
    void setLegatoEnabled(bool enabled);
    void setChordMemoryEnabled(bool enabled);
    void setMidiInputChannel(int channel);
    void setMidiOutputChannel(int channel);
    void setHumanizationAmount(float amount);

    // Monitoring/Analysis
    int getActiveVoiceCount() const;
    std::string getLastRecognizedChord() const;
    std::vector<int> getSuggestedChords() const;

private:
    // ============ Core Processing Modules ============
    ScaleMapper scaleMapper_;
    ChordVoicer chordVoicer_;
    Envelope envelope_;
    NoteTracker noteTracker_;
    MIDIEffects midiEffects_;
    ChordAnalyzer chordAnalyzer_;
    VoiceLeading voiceLeading_;
    JazzReharmonizer jazzReharmonizer_;
    PresetManager presetManager_;
    PerformanceDashboard dashboard_;

    // ============ APVTS (AudioProcessorValueTreeState) ============
    juce::AudioProcessorValueTreeState apvts_;

    // ============ Plugin Parameters (12 main automation parameters) ============
    // Music Parameters (4)
    int rootNote_ = 0;           // 0-11 (C-B)
    int scaleType_ = 0;          // 0-14 (15 scale types)
    int voicingType_ = 0;        // 0-3 (4 voicing algorithms)
    int octaveOffset_ = 0;       // -2 to +2

    // Envelope Parameters (4)
    float attackMs_ = 10.0f;           // 0-500 ms
    float decayMs_ = 100.0f;           // 0-500 ms
    float sustainLevel_ = 0.7f;        // 0.0-1.0
    float releaseMs_ = 300.0f;         // 0-1000 ms

    // MIDI Effects Parameters (4)
    bool legatoEnabled_ = false;
    bool chordMemoryEnabled_ = false;
    int noteDuration_ = 0;       // 0 = infinite, > 0 = duration
    float humanizationAmount_ = 0.05f; // 0.0-0.2

    // ============ MIDI Routing ============
    int midiInputChannel_ = 0;   // 0 = All channels, 1-16 = specific
    int midiOutputChannel_ = 0;  // 0-15 (JUCE 0-indexed)

    // ============ State & Caching ============
    double sampleRate_ = 44100.0;
    int samplesPerBlock_ = 256;
    int currentProgram_ = 0;
    std::string lastRecognizedChord_;
    std::vector<int> suggestedChords_;
    bool isDirty_ = true;

    // ============ Private Methods ============
    void updateSettings();
    void processNoteOn(int noteNumber, int velocity, int samplePosition, juce::MidiBuffer& outputBuffer);
    void processNoteOff(int noteNumber, int samplePosition, juce::MidiBuffer& outputBuffer);
    void processControlChange(int controller, int value, int samplePosition, juce::MidiBuffer& outputBuffer);
    void analyzeAndSuggest(int noteNumber);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};

} // namespace scalechord

#endif // JUCE_MODULE_AVAILABLE_juce_audio_processors
