# JUCE Integration Guide — ScaleChord v0.2

**Purpose**: Complete walkthrough for integrating the advanced ScaleChord core library into a JUCE VST3 plugin.

---

## 📋 Pre-Integration Checklist

- ✅ Core library built and tested (`libscalechord_core.a`)
- ✅ All headers in `include/` with clear API
- ✅ JUCE framework downloaded (v7.0+)
- ✅ Plugin requirements defined (VST3, AU, AAX targets)

---

## 🏗️ Step 1: Project Structure

```
scalechord-plugin-root/
├── CMakeLists.txt                 (main build config)
├── juce_config/
│   └── JuceConfig.cmake           (JUCE integration)
├── plugin/
│   ├── CMakeLists.txt            (existing core lib)
│   ├── include/                  (core headers)
│   ├── src/                       (core implementations)
│   └── tests/
├── juce_plugin/                   (NEW — VST3 wrapper)
│   ├── CMakeLists.txt
│   ├── include/
│   │   ├── PluginProcessor.h
│   │   ├── PluginEditor.h
│   │   └── PluginIds.h
│   ├── src/
│   │   ├── PluginProcessor.cpp
│   │   └── PluginEditor.cpp
│   └── resources/
│       └── icon.png
├── docs/
│   └── JUCE_INTEGRATION.md        (this file)
└── build/
```

---

## 📝 Step 2: Root CMakeLists.txt

Create `/workspaces/production-works/CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.16)
project(ScaleChordPlugin VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Determine if this is the first run
if(NOT DEFINED JUCE_PATH)
    message(FATAL_ERROR "JUCE_PATH not set. Run: cmake -DJUCE_PATH=/path/to/JUCE .")
endif()

# Build core library
add_subdirectory(plugin)

# Build JUCE plugin (if JUCE available)
if(EXISTS ${JUCE_PATH}/CMakeLists.txt)
    add_subdirectory(juce_plugin)
else()
    message(WARNING "JUCE not found at ${JUCE_PATH}. Skipping plugin build.")
endif()
```

**Build**:
```bash
cmake -DJUCE_PATH=/path/to/JUCE -B build
cmake --build build -j4
```

---

## 🎛️ Step 3: JUCE Plugin Structure

### **PluginIds.h**
```cpp
#pragma once

#define JucePlugin_Name                "ScaleChord"
#define JucePlugin_Desc                "Intelligent chord generator for non-musicians"
#define JucePlugin_Manufacturer        "ScaleChord Team"
#define JucePlugin_ManufacturerWebsite "https://scalechord.audio"
#define JucePlugin_ManufacturerEmail   "info@scalechord.audio"

#define JucePlugin_PluginCode          'SCRD'
#define JucePlugin_IsSynth             0
#define JucePlugin_WantsMidiInput      1
#define JucePlugin_ProducesMidiOutput  1
#define JucePlugin_SilenceInProducesSilenceOut 1
#define JucePlugin_TailLengthSeconds   2
#define JucePlugin_LatencyMs           5

#define JucePlugin_VST3Id              "scalechord.vst3"
#define JucePlugin_AUMainType          kAudioUnitType_MIDIProcessor
#define JucePlugin_AAXProductId        0x12345678

#define JucePlugin_VersionString       "0.2.0"
#define JucePlugin_VersionCode         0x020000
```

### **PluginProcessor.h**
```cpp
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "ScaleMapper.h"
#include "ChordVoicer.h"
#include "Envelope.h"
#include "NoteTracker.h"

using namespace juce;

class ScaleChordProcessor : public juce::AudioProcessor
{
public:
    ScaleChordProcessor();
    ~ScaleChordProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override;
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    bool isMidiEffect() const override { return true; }
    double getTailLengthSeconds() const override { return 2.0; }

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Parameters exposed to DAW
    juce::AudioParameterInt* midiChannelParam;
    juce::AudioParameterChoice* scaleParam;
    juce::AudioParameterInt* rootNoteParam;
    juce::AudioParameterChoice* voicingParam;
    juce::AudioParameterFloat* attackParam;
    juce::AudioParameterFloat* decayParam;
    juce::AudioParameterFloat* sustainParam;
    juce::AudioParameterFloat* releaseParam;
    juce::AudioParameterBool* humanizeParam;

private:
    // Core ScaleChord components
    ScaleMapper mapper_;
    ChordVoicer voicer_;
    Envelope envelope_;
    NoteTracker tracker_;

    double sampleRate_ = 44100.0;
    int midiChannel_ = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScaleChordProcessor)
};
```

### **PluginProcessor.cpp**
```cpp
#include "PluginProcessor.h"
#include "PluginEditor.h"

ScaleChordProcessor::ScaleChordProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Midi In", juce::AudioChannelSet::stereo(), true)
        .withOutput("Audio Out", juce::AudioChannelSet::stereo(), true))
{
    addParameter(midiChannelParam = new juce::AudioParameterInt(
        "midiChannel", "MIDI Channel", 1, 16, 1));
    
    addParameter(scaleParam = new juce::AudioParameterChoice(
        "scale", "Scale", {"Major", "Minor", "Harmonic Minor", 
                          "Dorian", "Phrygian", "Lydian", "Mixolydian",
                          "Melodic Minor", "Major Pentatonic", "Minor Pentatonic",
                          "Major Blues", "Minor Blues", "Whole Tone", "Diminished"}, 0));
    
    addParameter(rootNoteParam = new juce::AudioParameterInt(
        "root", "Root Note", 0, 127, 60));  // C4
    
    addParameter(voicingParam = new juce::AudioParameterChoice(
        "voicing", "Voicing", {"Triad", "Seventh", "Open"}, 0));
    
    addParameter(attackParam = new juce::AudioParameterFloat(
        "attack", "Attack (ms)", 0.0f, 1000.0f, 10.0f));
    
    addParameter(decayParam = new juce::AudioParameterFloat(
        "decay", "Decay (ms)", 0.0f, 1000.0f, 100.0f));
    
    addParameter(sustainParam = new juce::AudioParameterFloat(
        "sustain", "Sustain", 0.0f, 1.0f, 0.7f));
    
    addParameter(releaseParam = new juce::AudioParameterFloat(
        "release", "Release (ms)", 0.0f, 1000.0f, 300.0f));
    
    addParameter(humanizeParam = new juce::AudioParameterBool(
        "humanize", "Humanize", true));
}

ScaleChordProcessor::~ScaleChordProcessor() = default;

void ScaleChordProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    sampleRate_ = sampleRate;
    
    // Configure ScaleMapper
    MapperSettings ms;
    ms.rootNote = rootNoteParam->get();
    ms.scale = static_cast<ScaleType>(scaleParam->getIndex());
    mapper_ = ScaleMapper(ms);
    
    // Configure ChordVoicer
    ChordVoicerSettings cvs;
    cvs.quality = static_cast<VoicingQuality>(voicingParam->getIndex());
    voicer_ = ChordVoicer(cvs);
    
    // Configure Envelope
    EnvelopeSettings es;
    es.attack = attackParam->get();
    es.decay = decayParam->get();
    es.sustain = sustainParam->get();
    es.release = releaseParam->get();
    es.humanize = humanizeParam->get();
    envelope_ = Envelope(es);
}

void ScaleChordProcessor::releaseResources()
{
    // Cleanup if needed
}

void ScaleChordProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                       juce::MidiBuffer& midiMessages)
{
    juce::MidiBuffer outputMessages;
    
    // Process incoming MIDI
    for (const auto msg : midiMessages)
    {
        if (msg.isNoteOn())
        {
            int inputNote = msg.getNoteNumber();
            int velocity = msg.getVelocity();
            
            // Map to scale
            int mappedNote = mapper_.mapMidiNote(inputNote);
            
            // Generate chord
            std::vector<int> chord = voicer_.generateChord(mappedNote, 2);
            
            // Track notes
            tracker_.trackNoteOn(inputNote, chord, velocity);
            
            // Add envelope note-on
            envelope_.noteOn(velocity, sampleRate_);
            
            // Output chord notes
            for (int chordNote : chord)
            {
                outputMessages.addEvent(juce::MidiMessage::noteOn(
                    midiChannelParam->get(), chordNote, velocity), msg.samplePosition);
            }
        }
        else if (msg.isNoteOff())
        {
            int inputNote = msg.getNoteNumber();
            
            // Get note-offs from tracker
            auto noteOffs = tracker_.getNoteOffsForInputNote(inputNote);
            tracker_.trackNoteOff(inputNote);
            
            // Start envelope release
            envelope_.noteOff();
            
            // Output note-offs
            for (int chordNote : noteOffs)
            {
                outputMessages.addEvent(juce::MidiMessage::noteOff(
                    midiChannelParam->get(), chordNote), msg.samplePosition);
            }
        }
        else if (msg.isController() && msg.getControllerNumber() == 64)  // Sustain pedal
        {
            tracker_.setSustainPedal(msg.getControllerValue() > 63);
        }
    }
    
    // Apply envelope gain to output buffer
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float gain = envelope_.process();
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            buffer.getWritePointer(ch)[sample] *= gain;
        }
    }
    
    // Send output MIDI
    midiMessages = outputMessages;
}

juce::AudioProcessorEditor* ScaleChordProcessor::createEditor()
{
    return new ScaleChordEditor(*this);
}

const juce::String ScaleChordProcessor::getName() const
{
    return JucePlugin_Name;
}

// ... getStateInformation, setStateInformation, program management ...
```

### **PluginEditor.h**
```cpp
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

class ScaleChordEditor : public juce::AudioProcessorEditor
{
public:
    explicit ScaleChordEditor(ScaleChordProcessor&);
    ~ScaleChordEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    ScaleChordProcessor& processor_;
    
    // UI Components
    juce::ComboBox scaleSelector_;
    juce::ComboBox voicingSelector_;
    juce::ComboBox rootSelector_;
    juce::Slider attackSlider_;
    juce::Slider decaySlider_;
    juce::Slider sustainSlider_;
    juce::Slider releaseSlider_;
    juce::ToggleButton humanizeButton_;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScaleChordEditor)
};
```

### **PluginEditor.cpp**
```cpp
#include "PluginEditor.h"

ScaleChordEditor::ScaleChordEditor(ScaleChordProcessor& p)
    : AudioProcessorEditor(&p), processor_(p)
{
    setSize(600, 400);
    
    // Setup scale selector
    scaleSelector_.addItemList({"Major", "Minor", "Harmonic Minor", "Dorian", 
                                "Phrygian", "Lydian", "Mixolydian", "Melodic Minor",
                                "Major Pentatonic", "Minor Pentatonic", "Major Blues",
                                "Minor Blues", "Whole Tone", "Diminished"}, 1);
    scaleSelector_.setSelectedItemIndex(0);
    addAndMakeVisible(scaleSelector_);
    
    // Setup voicing selector
    voicingSelector_.addItemList({"Triad", "Seventh", "Open"}, 1);
    voicingSelector_.setSelectedItemIndex(0);
    addAndMakeVisible(voicingSelector_);
    
    // Setup ADSR sliders
    attackSlider_.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    attackSlider_.setRange(0.0f, 1000.0f);
    addAndMakeVisible(attackSlider_);
    
    // ... setup other controls ...
}

void ScaleChordEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(
        juce::ResizableWindow::backgroundColourId));
    
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("ScaleChord v0.2", getLocalBounds(),
                     juce::Justification::centredTop, 1);
}

void ScaleChordEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    
    scaleSelector_.setBounds(area.removeFromTop(40).withTrimmedLeft(100));
    voicingSelector_.setBounds(area.removeFromTop(40).withTrimmedLeft(100));
    
    // Arrange ADSR sliders in a row
    auto adsr = area.removeFromTop(100);
    attackSlider_.setBounds(adsr.removeFromLeft(area.getWidth() / 4));
    decaySlider_.setBounds(adsr.removeFromLeft(area.getWidth() / 4));
    sustainSlider_.setBounds(adsr.removeFromLeft(area.getWidth() / 4));
    releaseSlider_.setBounds(adsr.removeFromLeft(area.getWidth() / 4));
}
```

---

## 📦 Step 4: CMakeLists for JUCE Plugin

Create `/workspaces/production-works/juce_plugin/CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.16)
project(scalechord_juce_plugin)

include(${JUCE_PATH}/extras/Build/CMakeHelpers/JUCEHelpers.cmake)

juce_add_plugin(scalechord_plugin
    PRODUCT_NAME "ScaleChord"
    COMPANY_NAME "ScaleChord Team"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    
    VST3_CATEGORIES "MidiEffect" "Tools"
    AU_MAIN_TYPE "kAudioUnitType_MIDIProcessor"
    
    FORMATS VST3 AU # Add AAX if you have entitlements
    COPY_PLUGIN_AFTER_BUILD TRUE
)

target_sources(scalechord_plugin PRIVATE
    src/PluginProcessor.cpp
    src/PluginEditor.cpp
)

target_include_directories(scalechord_plugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../plugin/include
)

target_link_libraries(scalechord_plugin PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_gui_extra
    scalechord_core
)
```

---

## 🏗️ Step 5: Full Build & Test

```bash
# Clone JUCE
git clone https://github.com/juce-framework/JUCE.git /tmp/JUCE

# Configure
cd /workspaces/production-works
cmake -DJUCE_PATH=/tmp/JUCE -B build

# Build all targets
cmake --build build -j4

# Plugin locations
ls build/juce_plugin/scalechord_plugin_artefacts/
```

---

## 🧪 Step 6: Testing in DAW

1. **Locate VST3 Plugin**:
   ```bash
   ~/.vst3/scalechord_plugin.vst3/Contents/x86_64-linux/scalechord_plugin.so
   ```

2. **Open in DAW** (Reaper, Studio One, etc.):
   - Add plugin to MIDI track
   - Enable MIDI input
   - Enable MIDI output to another track with synth

3. **Play**:
   - Select scale (e.g., C Major)
   - Select voicing (Triad)
   - Adjust ADSR
   - Play one note → hears chord

---

## 🔌 Parameter Automation

All parameters expose to DAW automation:

```cpp
// In PluginProcessor constructor:
addParameter(attackParam = new juce::AudioParameterFloat(
    "attack", "Attack", 
    juce::NormalisableRange<float>(0.0f, 1000.0f, 1.0f, 0.5f),
    10.0f,
    "",
    juce::AudioProcessorParameter::Rate::perSecond));
```

DAW can automate attack, decay, sustain, release, scale, voicing, root note.

---

## 📊 State Management

Save/load plugin state for sessions:

```cpp
void ScaleChordProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto xml = std::make_unique<juce::XmlElement>("SCALECHORD");
    
    for (auto* param : getParameters())
        if (auto* p = dynamic_cast<juce::AudioProcessorParameter*>(param))
            xml->setAttribute(p->getParameterID(), p->getValue());
    
    copyXmlToBinary(*xml, destData);
}

void ScaleChordProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto xml = getXmlFromBinary(data, sizeInBytes);
    
    if (xml && xml->hasTagName("SCALECHORD"))
        for (auto* param : getParameters())
            if (auto* p = dynamic_cast<juce::AudioProcessorParameter*>(param))
                if (xml->hasAttribute(p->getParameterID()))
                    p->setValueNotifyingHost(xml->getDoubleAttribute(p->getParameterID()));
}
```

---

## 🎨 Advanced UI Components

See JUCE documentation for:
- Custom LookAndFeel for branding
- Waveform visualizer for envelope
- MIDI monitor display
- Preset browser
- Settings panel

---

## 🐛 Troubleshooting

| Issue | Solution |
|-------|----------|
| "JUCE not found" | Set `-DJUCE_PATH=/path/to/JUCE` |
| VST3 not loading | Check `~/.vst3/` or DAW plugin paths |
| MIDI not passing | Enable "MIDI through" in DAW settings |
| Envelope always 0 | Check `sampleRate_` is set in `prepareToPlay()` |
| Performance CPU spike | Profile with DAW profiler; optimize hot paths |

---

## ✅ Verification Checklist

- [ ] All JUCE plugin files in `juce_plugin/`
- [ ] CMakeLists.txt configured with JUCE_PATH
- [ ] Plugin builds without errors
- [ ] VST3 binary appears in plugin folder
- [ ] DAW recognizes plugin
- [ ] MIDI input routes to chord generation
- [ ] Parameters automate in DAW
- [ ] Sustain pedal (CC 64) works
- [ ] Plugin state saves/loads
- [ ] No CPU spikes during playback

---

## 🚀 Next Steps

1. **Skinning**: Customize UI with brand colors/fonts
2. **Presets**: Add preset browser with save/load
3. **Advanced Features**: MIDI learn, sidechain, arpeggiator UI
4. **Packaging**: Code sign, notarize for distribution
5. **Testing**: VST3 validator suite, compatibility testing

---

**Status**: Template complete. Ready for plugin development.  
**Estimated Integration Time**: 2–4 hours for full VST3 plugin.
