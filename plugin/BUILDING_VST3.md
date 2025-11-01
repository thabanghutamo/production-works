# Building ScaleChord as a VST3 Plugin

This guide walks you through taking the core library (already built and tested) and wrapping it into a production-ready VST3 plugin using JUCE.

## 📋 Prerequisites

- **JUCE 7.0+** (or JUCE 6.x, 8.x)
- **C++17 compiler** (GCC, Clang, MSVC)
- **CMake 3.20+** (if using CMake build)
- **VST3 SDK** (optional; JUCE bundles it)

## 🚀 Option 1: JUCE via Projucer (Recommended for Beginners)

### 1.1 Download & Build JUCE

```bash
git clone https://github.com/juce-framework/JUCE.git ~/JUCE
cd ~/JUCE/extras/Projucer

# Linux / Mac
./build_linux.sh
# Or open Projucer.xcodeproj on Mac

# Windows
# Open Projucer.sln in Visual Studio and build
```

### 1.2 Create a New Plugin Project

1. Launch **Projucer** (the GUI tool you just built)
2. **File** → **New Project** → **Audio Plug-in**
3. Fill in:
   - **Project Name**: `ScaleChordPlugin`
   - **Plugin Type**: VST3 (check VST3, optionally AU for Mac)
   - **Project Location**: Your desired folder
   - **Module Locations**: Point to your `~/JUCE` path
4. Click **Create Project**

### 1.3 Copy Core Library Files

In Projucer, your project structure looks like:
```
ScaleChordPlugin/
├── Source/
│   ├── PluginProcessor.h
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h
│   ├── PluginEditor.cpp
│   └── (generated files)
├── JuceLibraryCode/
└── ScaleChordPlugin.jucer
```

**Copy core files**:
```bash
# From the original plugin core
cp /workspaces/production-works/plugin/include/*.h \
   ScaleChordPlugin/Source/

cp /workspaces/production-works/plugin/src/*.cpp \
   ScaleChordPlugin/Source/
   # (Exclude demo_main.cpp and test files)
```

### 1.4 Integrate into JUCE Processor

Edit `ScaleChordPlugin/Source/PluginProcessor.h`:

```cpp
#pragma once

#include <JuceHeader.h>
#include "ScaleMapper.h"
#include "ChordVoicer.h"

using namespace scalechord;

class ScaleChordPluginAudioProcessor  : public juce::AudioProcessor
{
public:
    ScaleChordPluginAudioProcessor();
    ~ScaleChordPluginAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "ScaleChord"; }

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    bool isMidiEffect() const override { return true; }

    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return "Default"; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    // Audio parameters
    std::unique_ptr<juce::AudioProcessorValueTreeState> apvts;
    
    // Scale mapping
    ScaleMapper mapper_;
    ChordVoicer voicer_;
    
    // Settings
    int outputMidiChannel_ = 1;
    
    // Track active notes for proper note-off handling
    std::map<int, std::vector<int>> activeChordNotes_; // input note → generated notes
    
    void updateSettings();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScaleChordPluginAudioProcessor)
};
```

Edit `ScaleChordPlugin/Source/PluginProcessor.cpp`:

```cpp
#include "PluginProcessor.h"
#include "PluginEditor.h"

ScaleChordPluginAudioProcessor::ScaleChordPluginAudioProcessor()
{
    // Initialize parameters
    apvts = std::make_unique<juce::AudioProcessorValueTreeState>
    (*this, nullptr, "PARAMETERS", juce::AudioProcessorValueTreeState::ParameterLayout()
        .add(std::make_unique<juce::AudioParameterChoice>
             ("rootNote", "Root Note", 
              juce::StringArray{"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"}, 0))
        .add(std::make_unique<juce::AudioParameterChoice>
             ("scale", "Scale",
              juce::StringArray{"Major","Minor","Pentatonic","Dorian","Mixolydian"}, 0))
        .add(std::make_unique<juce::AudioParameterChoice>
             ("voicing", "Voicing",
              juce::StringArray{"Triad","Seventh","Open"}, 0))
        .add(std::make_unique<juce::AudioParameterFloat>
             ("attack", "Attack", 0.0f, 1000.0f, 10.0f))
        .add(std::make_unique<juce::AudioParameterFloat>
             ("release", "Release", 0.0f, 1000.0f, 100.0f))
        .add(std::make_unique<juce::AudioParameterInt>
             ("channel", "MIDI Channel", 1, 16, 1)));
}

ScaleChordPluginAudioProcessor::~ScaleChordPluginAudioProcessor() {}

void ScaleChordPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize mapper with defaults
    MapperSettings ms;
    ms.rootNote = 0;
    ms.scale = ScaleType::Major;
    mapper_ = ScaleMapper(ms);
    
    VoicerSettings vs;
    vs.voicing = VoicingType::Triad;
    voicer_ = ChordVoicer(mapper_);
    voicer_.setSettings(vs);
}

void ScaleChordPluginAudioProcessor::releaseResources() {}

void ScaleChordPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, 
                                                    juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    updateSettings();
    
    juce::MidiBuffer outputMidi;
    
    for (const auto metadata : midiMessages)
    {
        const auto msg = metadata.getMessage();
        
        if (msg.isNoteOn())
        {
            int incomingNote = msg.getNoteNumber();
            int velocity = msg.getVelocity();
            
            // Map and generate chord
            int mapped = mapper_.mapNote(incomingNote);
            auto chord = voicer_.makeChordFromNote(mapped);
            
            // Send note-ons
            for (int note : chord)
            {
                juce::MidiMessage noteOn = juce::MidiMessage::noteOn(outputMidiChannel_, note, (juce::uint8)velocity);
                outputMidi.addEvent(noteOn, metadata.samplePosition);
            }
            
            // Remember which notes we generated for this input
            activeChordNotes_[incomingNote] = chord;
        }
        else if (msg.isNoteOff())
        {
            int incomingNote = msg.getNoteNumber();
            
            // Send note-offs for all generated notes
            if (activeChordNotes_.find(incomingNote) != activeChordNotes_.end())
            {
                for (int note : activeChordNotes_[incomingNote])
                {
                    juce::MidiMessage noteOff = juce::MidiMessage::noteOff(outputMidiChannel_, note);
                    outputMidi.addEvent(noteOff, metadata.samplePosition);
                }
                activeChordNotes_.erase(incomingNote);
            }
        }
        else
        {
            // Pass through other MIDI messages
            outputMidi.addEvent(msg, metadata.samplePosition);
        }
    }
    
    midiMessages.swapWith(outputMidi);
}

void ScaleChordPluginAudioProcessor::updateSettings()
{
    // Read APVTS parameters
    int rootNote = apvts->getParameter("rootNote")->getValue() * 11; // 0..11
    int scaleIdx = apvts->getParameter("scale")->getValue() * 4; // 0..4
    int voicingIdx = apvts->getParameter("voicing")->getValue() * 2; // 0..2
    outputMidiChannel_ = apvts->getParameter("channel")->getValue() * 15 + 1; // 1..16
    
    // Update scale
    MapperSettings ms;
    ms.rootNote = rootNote;
    ms.scale = static_cast<ScaleType>(scaleIdx);
    mapper_.setSettings(ms);
    
    // Update voicing
    VoicerSettings vs;
    vs.voicing = static_cast<VoicingType>(voicingIdx);
    voicer_.setSettings(vs);
}

juce::AudioProcessorEditor* ScaleChordPluginAudioProcessor::createEditor()
{
    return new ScaleChordPluginAudioProcessorEditor (*this);
}

void ScaleChordPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts->copyValueTree();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void ScaleChordPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState && xmlState->hasTagName (apvts->state.getType()))
        apvts->replaceState (juce::ValueTree::fromXml (*xmlState));
}
```

### 1.5 Create the UI (PluginEditor)

Edit `ScaleChordPlugin/Source/PluginEditor.h`:

```cpp
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ScaleChordPluginAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                              public juce::Slider::Listener
{
public:
    ScaleChordPluginAudioProcessorEditor (ScaleChordPluginAudioProcessor&);
    ~ScaleChordPluginAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* slider) override;

private:
    ScaleChordPluginAudioProcessor& processor;
    
    juce::ComboBox rootNoteBox_;
    juce::ComboBox scaleBox_;
    juce::ComboBox voicingBox_;
    juce::ComboBox channelBox_;
    juce::Slider attackSlider_, releaseSlider_;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> rootNoteAttach_, scaleAttach_, voicingAttach_, channelAttach_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttach_, releaseAttach_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScaleChordPluginAudioProcessorEditor)
};
```

(See `plugin/integration/` for a more complete UI example.)

### 1.6 Save & Export from Projucer

1. In Projucer, **File** → **Save Project**
2. **Projucer** → **File** → **Open Exporter** → Select your platform:
   - **Linux Makefile**
   - **Xcode** (Mac)
   - **Visual Studio** (Windows)
3. Choose build folder and **Export**

### 1.7 Build

```bash
# Linux
cd Builds/LinuxMakeFile
make -j CONFIG=Release

# Mac
cd Builds/MacOSX
xcodebuild -scheme ScaleChordPlugin -configuration Release

# Windows (Visual Studio)
cd Builds\VisualStudio2022
msbuild ScaleChordPlugin.sln /p:Configuration=Release /p:Platform=x64
```

### 1.8 Locate Your VST3 Plugin

```bash
# After successful build:
# Linux: plugin appears in Builds/LinuxMakeFile/build/ScaleChordPlugin_artefacts/Release/VST3/
# Mac: plugin appears as ScaleChordPlugin.vst3 in Builds/MacOSX/build/Release/
# Windows: plugin in Builds\VisualStudio2022\Builds\Release\VST3\
```

### 1.9 Install to DAW

Copy `.vst3` to your DAW's plugin folder:

**Linux**:
```bash
cp build/ScaleChordPlugin.vst3 ~/.vst3/
```

**Mac**:
```bash
cp -r build/ScaleChordPlugin.vst3 /Library/Audio/Plug-Ins/VST3/
```

**Windows**:
```bash
copy build\ScaleChordPlugin.vst3 "C:\Program Files\Common Files\VST3\"
```

## 🎯 Option 2: CMake Build

(Alternative to Projucer; advanced users)

```bash
mkdir -p build && cd build
cmake .. -DJUCE_INSTALL_PATH=~/JUCE -DCMAKE_BUILD_TYPE=Release
cmake --build . -- -j
# VST3 plugin in: build/ScaleChordPlugin_artefacts/Release/VST3/
```

## ✅ Verification

1. **Rescan plugins** in your DAW
2. **Load ScaleChord** on an instrument track
3. **Add a synth** (Serum, Wavetable, etc.) after it
4. **Set MIDI output channel** to 1 (or wherever your synth listens)
5. **Play notes** on your keyboard
6. **Hear chords** from the synth

If it works, you have a functioning VST3 plugin!

## 📦 Distribution (Optional)

Once working, package for release:

### Windows: NSIS Installer

```bash
# Use NSIS to create an .exe installer that copies VST3 to:
# C:\Program Files\Common Files\VST3\
```

### Mac: Code Signing & Notarization

```bash
# Sign and notarize the .vst3 for distribution on macOS
codesign --force --verify --verbose --sign "Developer ID Application: Your Name" \
    build/ScaleChordPlugin.vst3

# Submit to Apple for notarization
xcrun altool --notarize-app -f build/ScaleChordPlugin.vst3 ...
```

### Linux: RPM/DEB Packages

```bash
# Use alien or native package tools to create .deb or .rpm
```

---

**Troubleshooting**: See `QUICKSTART.md` and DAW-specific guides in `MIDI_ROUTING.md`.
