#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginIds.h"

using namespace scalechord;

// Include ScaleChord core headers
#include "../../../include/ScaleMapper.h"
#include "../../../include/ChordVoicer.h"
#include "../../../include/VoiceLeading.h"
#include "../../../include/ChordAnalyzer.h"
#include "../../../include/JazzReharmonizer.h"
#include "../../../include/NoteTracker.h"
#include "../../../include/MIDIEffects.h"

// ============================================================================
// Constructor & Destructor
// ============================================================================

ScaleChordPluginAudioProcessor::ScaleChordPluginAudioProcessor()
    : parameters(*this, nullptr, juce::Identifier("ScaleChord"), 
                 createParameterLayout())
{
    // Add listener for parameter changes
    parameters.addParameterListener("scale", this);
    parameters.addParameterListener("voiceLeading", this);
    parameters.addParameterListener("midiEffects", this);
    parameters.addParameterListener("jazzReharmonization", this);

    // Initialize core modules
    initializeCoreModules();

    // Initialize active notes
    activeNotes.fill(-1);
    noteVoicings.fill(-1);

    DBG("ScaleChordPluginAudioProcessor created");
}

ScaleChordPluginAudioProcessor::~ScaleChordPluginAudioProcessor()
{
    DBG("ScaleChordPluginAudioProcessor destroyed");
}

// ============================================================================
// Audio Processing
// ============================================================================

void ScaleChordPluginAudioProcessor::prepareToPlay(double sampleRate_, int samplesPerBlock_)
{
    sampleRate = sampleRate_;
    bufferSize = samplesPerBlock_;

    DBG("Preparing to play:");
    DBG("  Sample rate: " << sampleRate);
    DBG("  Buffer size: " << bufferSize);

    // Core modules are initialized and ready for processing
}

void ScaleChordPluginAudioProcessor::releaseResources()
{
    DBG("Releasing resources");
}

void ScaleChordPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                   juce::MidiBuffer& midiMessages)
{
    // Process all MIDI messages in this block
    for (auto metadata : midiMessages)
    {
        auto msg = metadata.getMessage();

        if (msg.isNoteOn())
        {
            processMidiNoteOn(msg.getNoteNumber(), msg.getVelocity());
        }
        else if (msg.isNoteOff())
        {
            processMidiNoteOff(msg.getNoteNumber(), msg.getVelocity());
        }
        else if (msg.isController())
        {
            // Handle MIDI CC if needed
            auto cc = msg.getControllerNumber();
            auto value = msg.getControllerValue();

            // Could implement MIDI mapping here
            DBG("MIDI CC: " << cc << " = " << value);
        }
    }

    // Apply voice leading
    applyVoiceLeading(midiMessages);

    // Apply MIDI effects if enabled
    if (midiEffectsEnabled && midiEffects)
    {
        // Apply effects to MIDI output
    }

    // Clear audio buffer (this is a MIDI effect, not audio generator)
    buffer.clear();
}

// ============================================================================
// MIDI Processing
// ============================================================================

void ScaleChordPluginAudioProcessor::processMidiNoteOn(int noteNumber, int velocity)
{
    if (!chordVoicer)
        return;

    DBG("Note On: " << noteNumber << " Velocity: " << velocity);

    // Store active note
    if (noteNumber < 128)
        activeNotes[noteNumber / 16] = noteNumber;

    // Generate voicing for this note
    auto voicing = generateVoicing(noteNumber);

    DBG("Generated voicing with " << voicing.size() << " notes");
}

void ScaleChordPluginAudioProcessor::processMidiNoteOff(int noteNumber, int velocity)
{
    if (noteNumber < 128)
        activeNotes[noteNumber / 16] = -1;

    DBG("Note Off: " << noteNumber);
}

void ScaleChordPluginAudioProcessor::applyVoiceLeading(juce::MidiBuffer& outputMidi)
{
    if (!voiceLeading || voiceLeadingMode == 0)
        return;

    // Apply voice leading algorithm
    // This reduces jumps between voicings for smooth playback
}

// ============================================================================
// ScaleChord Core Integration
// ============================================================================

void ScaleChordPluginAudioProcessor::setScale(const std::string& newScale)
{
    if (currentScale != newScale)
    {
        currentScale = newScale;
        onScaleChanged();
        DBG("Scale changed to: " << newScale);
    }
}

void ScaleChordPluginAudioProcessor::setVoiceLeadingMode(int mode)
{
    voiceLeadingMode = mode;
    DBG("Voice leading mode: " << mode);
}

std::vector<int> ScaleChordPluginAudioProcessor::generateVoicing(int rootNote)
{
    std::vector<int> voicing;

    if (!chordVoicer)
        return voicing;

    // Use the ScaleChord chord voicer to generate intelligent voicing
    // This is where the magic happens!

    return voicing;
}

void ScaleChordPluginAudioProcessor::onScaleChanged()
{
    // Re-voice all active notes with the new scale
    for (size_t i = 0; i < activeNotes.size(); ++i)
    {
        int note = activeNotes[i];
        if (note >= 0)
        {
            auto voicing = generateVoicing(note);
            DBG("Re-voiced note: " << note);
        }
    }
}

// ============================================================================
// Parameter Management
// ============================================================================

juce::AudioProcessorValueTreeState::ParameterLayout
ScaleChordPluginAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Scale selector (0-12 for different scales)
    params.push_back(std::make_unique<juce::AudioParameterInt>(
        "scale",
        "Scale",
        0, 12, 0));

    // Voice leading mode (0-3)
    params.push_back(std::make_unique<juce::AudioParameterInt>(
        "voiceLeading",
        "Voice Leading",
        0, 3, 1));

    // MIDI Effects toggle
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        "midiEffects",
        "MIDI Effects",
        true));

    // Jazz Reharmonization toggle
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        "jazzReharmonization",
        "Jazz Reharmonization",
        false));

    // Voicing threshold
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "voicingThreshold",
        "Voicing Threshold",
        0.0f, 1.0f, 0.5f));

    // Analysis smoothing
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "analyzingSmoothing",
        "Analysis Smoothing",
        0.0f, 1.0f, 0.3f));

    // Polyphony limit
    params.push_back(std::make_unique<juce::AudioParameterInt>(
        "polyphony",
        "Polyphony",
        1, 16, 8));

    return { params.begin(), params.end() };
}

void ScaleChordPluginAudioProcessor::parameterChanged(const juce::String& parameterID,
                                                       float newValue)
{
    if (parameterID == "scale")
    {
        // TODO: Update scale
    }
    else if (parameterID == "voiceLeading")
    {
        setVoiceLeadingMode(static_cast<int>(newValue));
    }
    else if (parameterID == "midiEffects")
    {
        setMidiEffectsEnabled(newValue > 0.5f);
    }
    else if (parameterID == "jazzReharmonization")
    {
        setJazzReharmonizationEnabled(newValue > 0.5f);
    }
}

void ScaleChordPluginAudioProcessor::initializeCoreModules()
{
    DBG("Initializing ScaleChord core modules...");

    try
    {
        // ScaleMapper must be initialized first (dependency for ChordVoicer)
        scaleMapper = std::make_unique<ScaleMapper>();
        
        // ChordVoicer depends on ScaleMapper
        chordVoicer = std::make_unique<ChordVoicer>(*scaleMapper);
        
        // Other independent modules
        voiceLeading = std::make_unique<VoiceLeading>();
        chordAnalyzer = std::make_unique<ChordAnalyzer>();
        jazzReharmonizer = std::make_unique<JazzReharmonizer>();
        noteTracker = std::make_unique<NoteTracker>();

        DBG("✓ All core modules initialized successfully");
    }
    catch (const std::exception& e)
    {
        DBG("✗ Error initializing core modules: " << e.what());
    }
}

// ============================================================================
// State Serialization
// ============================================================================

void ScaleChordPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.state;
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ScaleChordPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
}

// ============================================================================
// Factory Method
// ============================================================================

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ScaleChordPluginAudioProcessor();
}
