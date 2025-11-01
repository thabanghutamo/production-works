#if defined(JUCE_MODULE_AVAILABLE_juce_audio_processors)

#include "PluginProcessor.h"
#include <algorithm>
#include <sstream>

using namespace scalechord;

// ============================================================================
// Constructor & Destructor
// ============================================================================

PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    // Initialize all factory presets
    presetManager_.loadFactoryPresets();
    
    // Load first factory preset (Bebop)
    if (auto preset = presetManager_.getPreset("Bebop")) {
        loadPreset(*preset);
    }

    // Initialize MIDI note tracker for polyphonic handling
    noteTracker_.initialize(16);  // 16 simultaneous voices
}

PluginProcessor::~PluginProcessor() = default;

// ============================================================================
// AudioProcessor Core Overrides
// ============================================================================

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    sampleRate_ = sampleRate;
    samplesPerBlock_ = samplesPerBlock;

    // Initialize all module-specific settings
    updateSettings();

    // Prepare envelope with sample rate
    envelope_.setSampleRate(sampleRate);

    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void PluginProcessor::releaseResources()
{
    // Clean up any real-time resources
    noteTracker_.reset();
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Clear output if not bypassed
    buffer.clear();

    juce::MidiBuffer processedMidi;

    // Iterate through incoming MIDI messages
    for (const auto metadata : midiMessages)
    {
        const auto msg = metadata.getMessage();

        // Check MIDI channel routing
        if (midiInputChannel_ != 0 && msg.getChannel() != midiInputChannel_) {
            processedMidi.addEvent(msg, metadata.samplePosition);
            continue;
        }

        if (msg.isNoteOn())
        {
            processNoteOn(msg.getNoteNumber(), msg.getVelocity(), 
                         metadata.samplePosition, processedMidi);
        }
        else if (msg.isNoteOff())
        {
            processNoteOff(msg.getNoteNumber(), metadata.samplePosition, processedMidi);
        }
        else if (msg.isController())
        {
            processControlChange(msg.getControllerNumber(), msg.getControllerValue(),
                                metadata.samplePosition, processedMidi);
        }
        else
        {
            // Pass through other MIDI messages
            processedMidi.addEvent(msg, metadata.samplePosition);
        }
    }

    // Replace incoming MIDI with processed output
    midiMessages.swapWith(processedMidi);
}

void PluginProcessor::processBlockBypassed(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Pass through audio and MIDI unchanged
    buffer.clear();
    juce::ignoreUnused(midiMessages);
}

// ============================================================================
// MIDI Processing Methods
// ============================================================================

void PluginProcessor::processNoteOn(int noteNumber, int velocity, int samplePosition, 
                                    juce::MidiBuffer& outputBuffer)
{
    // Analyze incoming note for chord recognition
    analyzeAndSuggest(noteNumber);

    // Map incoming note to scale
    int mappedNote = scaleMapper_.mapNote(noteNumber);

    // Generate chord from mapped note
    auto chord = chordVoicer_.makeChordFromNote(mappedNote);

    // Track the note internally
    noteTracker_.noteOn(noteNumber, velocity);

    // Apply voice leading if multiple voices
    if (chord.size() > 1) {
        chord = voiceLeading_.optimizeVoicing(chord, voiceLeading_.getLastVoicing());
    }

    // Check for jazz reharmonization
    if (scaleType_ >= 8) {  // Jazz/advanced scales
        auto reharmonized = jazzReharmonizer_.reharmonize(chord);
        if (!reharmonized.empty()) {
            chord = reharmonized;
        }
    }

    // Send note-ons for generated chord
    for (int note : chord)
    {
        juce::MidiMessage m = juce::MidiMessage::noteOn(
            midiOutputChannel_ + 1,  // JUCE uses 1-16
            note,
            static_cast<juce::uint8>(velocity));
        outputBuffer.addEvent(m, samplePosition);
    }

    // Envelope attack
    envelope_.noteOn(velocity / 127.0f);
}

void PluginProcessor::processNoteOff(int noteNumber, int samplePosition, 
                                     juce::MidiBuffer& outputBuffer)
{
    // Get the generated notes for this input note
    int mappedNote = scaleMapper_.mapNote(noteNumber);
    auto chord = chordVoicer_.makeChordFromNote(mappedNote);

    // Send note-offs for all generated notes
    for (int note : chord)
    {
        juce::MidiMessage m = juce::MidiMessage::noteOff(
            midiOutputChannel_ + 1,
            note,
            static_cast<juce::uint8>(0));
        outputBuffer.addEvent(m, samplePosition);
    }

    // Track note off
    noteTracker_.noteOff(noteNumber);

    // Envelope release
    envelope_.noteOff();
}

void PluginProcessor::processControlChange(int controller, int value, int samplePosition, 
                                           juce::MidiBuffer& outputBuffer)
{
    // Map MIDI CC to plugin parameters
    switch (controller)
    {
        case 0:   // Bank Select - for preset changes
            break;
        case 32:  // LSB for Bank Select
            break;
        case 120: // All Sounds Off
            noteTracker_.reset();
            envelope_.reset();
            break;
        case 123: // All Notes Off
            noteTracker_.allNotesOff();
            break;
        default:
            // Pass through unmapped CCs
            juce::MidiMessage m = juce::MidiMessage::controllerEvent(
                midiOutputChannel_ + 1, controller, value);
            outputBuffer.addEvent(m, samplePosition);
            break;
    }

    juce::ignoreUnused(samplePosition);
}

void PluginProcessor::analyzeAndSuggest(int noteNumber)
{
    // Use ChordAnalyzer to recognize chord from incoming note
    std::vector<int> recognizedChord;
    recognizedChord.push_back(noteNumber);
    
    // Generate suggested complementary chords
    auto analyzed = chordAnalyzer_.analyzeChord(recognizedChord);
    lastRecognizedChord_ = analyzed;

    // Get reharmonization suggestions
    suggestedChords_ = jazzReharmonizer_.suggestChords(noteNumber);
}

// ============================================================================
// Parameter Management (12 Automation Parameters)
// ============================================================================

int PluginProcessor::getNumParameters() const
{
    return 12;  // Root, Scale, Voicing, Octave, Attack, Decay, Sustain, Release, 
                // Legato, ChordMemory, InputCh, OutputCh
}

float PluginProcessor::getParameter(int index) const
{
    switch (index)
    {
        case 0:  return rootNote_ / 11.0f;           // 0-1
        case 1:  return scaleType_ / 14.0f;          // 0-1
        case 2:  return voicingType_ / 3.0f;         // 0-1
        case 3:  return (octaveOffset_ + 2) / 4.0f;  // 0-1
        case 4:  return attackMs_ / 500.0f;          // 0-1 (0-500ms)
        case 5:  return decayMs_ / 500.0f;           // 0-1 (0-500ms)
        case 6:  return sustainLevel_;               // 0-1
        case 7:  return releaseMs_ / 1000.0f;        // 0-1 (0-1000ms)
        case 8:  return legatoEnabled_ ? 1.0f : 0.0f;
        case 9:  return chordMemoryEnabled_ ? 1.0f : 0.0f;
        case 10: return midiInputChannel_ / 16.0f;   // 0-1
        case 11: return midiOutputChannel_ / 15.0f;  // 0-1
        default: return 0.0f;
    }
}

void PluginProcessor::setParameter(int index, float newValue)
{
    newValue = juce::jlimit(0.0f, 1.0f, newValue);

    switch (index)
    {
        case 0:  rootNote_ = static_cast<int>(newValue * 11.0f + 0.5f); break;
        case 1:  scaleType_ = static_cast<int>(newValue * 14.0f + 0.5f); break;
        case 2:  voicingType_ = static_cast<int>(newValue * 3.0f + 0.5f); break;
        case 3:  octaveOffset_ = static_cast<int>(newValue * 4.0f + 0.5f - 2.0f); break;
        case 4:  attackMs_ = newValue * 500.0f; break;
        case 5:  decayMs_ = newValue * 500.0f; break;
        case 6:  sustainLevel_ = newValue; break;
        case 7:  releaseMs_ = newValue * 1000.0f; break;
        case 8:  legatoEnabled_ = newValue > 0.5f; break;
        case 9:  chordMemoryEnabled_ = newValue > 0.5f; break;
        case 10: midiInputChannel_ = static_cast<int>(newValue * 16.0f + 0.5f); break;
        case 11: midiOutputChannel_ = static_cast<int>(newValue * 15.0f + 0.5f); break;
        default: break;
    }

    isDirty_ = true;
}

const juce::String PluginProcessor::getParameterName(int index) const
{
    switch (index)
    {
        case 0:  return "Root Note";
        case 1:  return "Scale Type";
        case 2:  return "Voicing";
        case 3:  return "Octave";
        case 4:  return "Attack";
        case 5:  return "Decay";
        case 6:  return "Sustain";
        case 7:  return "Release";
        case 8:  return "Legato";
        case 9:  return "Chord Memory";
        case 10: return "MIDI Input Channel";
        case 11: return "MIDI Output Channel";
        default: return "";
    }
}

const juce::String PluginProcessor::getParameterText(int index) const
{
    switch (index)
    {
        case 0: {
            static const char* notes[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
            return juce::String(notes[rootNote_]);
        }
        case 1: {
            static const char* scales[] = {"Major", "Minor", "Dorian", "Phrygian", "Lydian", 
                                          "Mixolydian", "Aeolian", "Locrian", "Major Pent", 
                                          "Minor Pent", "Blues", "Harmonic Min", "Harmonic Maj",
                                          "Whole Tone", "Diminished"};
            return juce::String(scales[scaleType_]);
        }
        case 2: {
            static const char* voicings[] = {"Fundamental", "Shell", "Drop2", "Rootless"};
            return juce::String(voicings[voicingType_]);
        }
        case 3:  return juce::String(octaveOffset_);
        case 4:  return juce::String(static_cast<int>(attackMs_)) + "ms";
        case 5:  return juce::String(static_cast<int>(decayMs_)) + "ms";
        case 6:  return juce::String(sustainLevel_, 2);
        case 7:  return juce::String(static_cast<int>(releaseMs_)) + "ms";
        case 8:  return legatoEnabled_ ? "On" : "Off";
        case 9:  return chordMemoryEnabled_ ? "On" : "Off";
        case 10: return midiInputChannel_ == 0 ? "All" : juce::String(midiInputChannel_);
        case 11: return juce::String(midiOutputChannel_ + 1);
        default: return "";
    }
}

// ============================================================================
// Program/Preset Management
// ============================================================================

int PluginProcessor::getNumPrograms() const
{
    return static_cast<int>(presetManager_.getAllPresets().size());
}

int PluginProcessor::getCurrentProgram() const
{
    return currentProgram_;
}

void PluginProcessor::setCurrentProgram(int index)
{
    currentProgram_ = index;
    
    auto allPresets = presetManager_.getAllPresets();
    if (index >= 0 && index < static_cast<int>(allPresets.size())) {
        loadPreset(allPresets[index]);
    }
}

const juce::String PluginProcessor::getProgramName(int index) const
{
    auto allPresets = presetManager_.getAllPresets();
    if (index >= 0 && index < static_cast<int>(allPresets.size())) {
        return juce::String(allPresets[index].name);
    }
    return "Unknown";
}

void PluginProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
    // Implementation would require preset modification
}

// ============================================================================
// State Management
// ============================================================================

void PluginProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // Serialize current settings as JSON
    std::stringstream ss;
    ss << "{\n";
    ss << "  \"rootNote\": " << rootNote_ << ",\n";
    ss << "  \"scaleType\": " << scaleType_ << ",\n";
    ss << "  \"voicingType\": " << voicingType_ << ",\n";
    ss << "  \"attackMs\": " << attackMs_ << ",\n";
    ss << "  \"decayMs\": " << decayMs_ << ",\n";
    ss << "  \"sustainLevel\": " << sustainLevel_ << ",\n";
    ss << "  \"releaseMs\": " << releaseMs_ << ",\n";
    ss << "  \"legatoEnabled\": " << (legatoEnabled_ ? "true" : "false") << ",\n";
    ss << "  \"chordMemoryEnabled\": " << (chordMemoryEnabled_ ? "true" : "false") << ",\n";
    ss << "  \"midiInputChannel\": " << midiInputChannel_ << ",\n";
    ss << "  \"midiOutputChannel\": " << midiOutputChannel_ << "\n";
    ss << "}\n";

    std::string stateJson = ss.str();
    destData.replaceAll(stateJson.data(), stateJson.size());
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // Parse JSON state and restore
    juce::String stateStr(reinterpret_cast<const char*>(data), sizeInBytes);
    
    // Simple JSON parsing (in production, use robust JSON library)
    // For now, we just restore defaults
    updateSettings();
}

// ============================================================================
// Preset Management
// ============================================================================

void PluginProcessor::loadPreset(const Preset& preset)
{
    rootNote_ = preset.musicSettings.rootNote;
    scaleType_ = preset.musicSettings.scale;
    voicingType_ = preset.musicSettings.voicing;
    octaveOffset_ = preset.musicSettings.octaveOffset;
    
    attackMs_ = preset.envelopeSettings.attack;
    decayMs_ = preset.envelopeSettings.decay;
    sustainLevel_ = preset.envelopeSettings.sustain;
    releaseMs_ = preset.envelopeSettings.release;
    
    humanizationAmount_ = preset.envelopeSettings.humanizeAmount;

    isDirty_ = true;
    updateSettings();
}

void PluginProcessor::saveCurrentAsPreset(const std::string& name, const std::string& category)
{
    Preset preset;
    preset.name = name;
    preset.category = category;
    preset.author = "ScaleChord";
    
    preset.musicSettings.rootNote = rootNote_;
    preset.musicSettings.scale = scaleType_;
    preset.musicSettings.voicing = voicingType_;
    preset.musicSettings.octaveOffset = octaveOffset_;
    
    preset.envelopeSettings.attack = attackMs_;
    preset.envelopeSettings.decay = decayMs_;
    preset.envelopeSettings.sustain = sustainLevel_;
    preset.envelopeSettings.release = releaseMs_;
    preset.envelopeSettings.humanizeAmount = humanizationAmount_;

    presetManager_.addPreset(preset);
}

Preset PluginProcessor::getCurrentPreset() const
{
    Preset current;
    current.name = "Current";
    current.musicSettings.rootNote = rootNote_;
    current.musicSettings.scale = scaleType_;
    current.musicSettings.voicing = voicingType_;
    current.musicSettings.octaveOffset = octaveOffset_;
    
    current.envelopeSettings.attack = attackMs_;
    current.envelopeSettings.decay = decayMs_;
    current.envelopeSettings.sustain = sustainLevel_;
    current.envelopeSettings.release = releaseMs_;
    current.envelopeSettings.humanizeAmount = humanizationAmount_;

    return current;
}

// ============================================================================
// Parameter Direct Access (for automation/UI)
// ============================================================================

void PluginProcessor::setRootNote(int note) { rootNote_ = juce::jlimit(0, 11, note); isDirty_ = true; }
void PluginProcessor::setScaleType(int scaleIdx) { scaleType_ = juce::jlimit(0, 14, scaleIdx); isDirty_ = true; }
void PluginProcessor::setVoicingType(int voicingIdx) { voicingType_ = juce::jlimit(0, 3, voicingIdx); isDirty_ = true; }
void PluginProcessor::setAttackMs(float ms) { attackMs_ = juce::jlimit(0.0f, 500.0f, ms); isDirty_ = true; }
void PluginProcessor::setDecayMs(float ms) { decayMs_ = juce::jlimit(0.0f, 500.0f, ms); isDirty_ = true; }
void PluginProcessor::setSustainLevel(float level) { sustainLevel_ = juce::jlimit(0.0f, 1.0f, level); isDirty_ = true; }
void PluginProcessor::setReleaseMs(float ms) { releaseMs_ = juce::jlimit(0.0f, 1000.0f, ms); isDirty_ = true; }
void PluginProcessor::setLegatoEnabled(bool enabled) { legatoEnabled_ = enabled; isDirty_ = true; }
void PluginProcessor::setChordMemoryEnabled(bool enabled) { chordMemoryEnabled_ = enabled; isDirty_ = true; }
void PluginProcessor::setMidiInputChannel(int channel) { midiInputChannel_ = juce::jlimit(0, 16, channel); isDirty_ = true; }
void PluginProcessor::setMidiOutputChannel(int channel) { midiOutputChannel_ = juce::jlimit(0, 15, channel); isDirty_ = true; }
void PluginProcessor::setHumanizationAmount(float amount) { humanizationAmount_ = juce::jlimit(0.0f, 0.2f, amount); isDirty_ = true; }

// ============================================================================
// Monitoring/Analysis
// ============================================================================

int PluginProcessor::getActiveVoiceCount() const
{
    return noteTracker_.getActiveNoteCount();
}

std::string PluginProcessor::getLastRecognizedChord() const
{
    return lastRecognizedChord_;
}

std::vector<int> PluginProcessor::getSuggestedChords() const
{
    return suggestedChords_;
}

// ============================================================================
// Private Methods
// ============================================================================

void PluginProcessor::updateSettings()
{
    if (!isDirty_) return;

    // Update ScaleMapper
    ScaleMapper::MapperSettings mapperSettings;
    mapperSettings.rootNote = rootNote_;
    mapperSettings.scaleType = static_cast<ScaleType>(scaleType_);
    scaleMapper_.setSettings(mapperSettings);

    // Update ChordVoicer
    ChordVoicer::VoicerSettings voicerSettings;
    voicerSettings.voicingType = static_cast<VoicingType>(voicingType_);
    chordVoicer_.setSettings(voicerSettings);

    // Update Envelope
    envelope_.setADSR(attackMs_, decayMs_, sustainLevel_, releaseMs_);

    // Update MIDI Effects
    MIDIEffects::Settings effectsSettings;
    effectsSettings.legatoEnabled = legatoEnabled_;
    effectsSettings.chordMemoryEnabled = chordMemoryEnabled_;
    midiEffects_.setSettings(effectsSettings);

    isDirty_ = false;
}

#endif // JUCE_MODULE_AVAILABLE_juce_audio_processors
