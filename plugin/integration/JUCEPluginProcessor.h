// Example JUCE AudioProcessor integration (illustrative). Requires JUCE 6/7/8 to compile.
// This file demonstrates how to wire ScaleMapper and ChordVoicer into a real-time
// processBlock to translate incoming MIDI notes into scale-corrected chords.

#pragma once

#if defined(JUCE_MODULE_AVAILABLE_juce_audio_processors)
#include <juce_audio_processors/juce_audio_processors.h>
#else
#error "This example requires JUCE. Include this file only after adding JUCE to the project."
#endif

#include "../include/ScaleMapper.h"
#include "../include/ChordVoicer.h"

class ScaleChordAudioProcessor  : public juce::AudioProcessor
{
public:
    ScaleChordAudioProcessor();
    ~ScaleChordAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override { return nullptr; }
    bool hasEditor() const override { return false; }

    const juce::String getName() const override { return "ScaleChord"; }
    double getTailLengthSeconds() const override { return 0.0; }

    // ... other overrides omitted for brevity

private:
    // plugin parameters (to be hooked into AudioProcessorValueTreeState in real plugin)
    scalechord::MapperSettings mapperSettings_;
    scalechord::VoicerSettings voicerSettings_;

    scalechord::ScaleMapper mapper_{mapperSettings_};
    scalechord::ChordVoicer voicer_{mapper_};

    int targetMidiChannel_ = 1; // 1..16

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScaleChordAudioProcessor)
};

#endif // JUCE_MODULE_AVAILABLE_juce_audio_processors
