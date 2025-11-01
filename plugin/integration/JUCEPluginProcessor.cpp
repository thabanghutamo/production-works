// Implementation sketch for the JUCE-style AudioProcessor. This file is intentionally
// illustrative and omits full parameter-state management and GUI code. Use it as a
// reference when integrating into a real JUCE plugin project.

#if defined(JUCE_MODULE_AVAILABLE_juce_audio_processors)
#include "JUCEPluginProcessor.h"

using namespace scalechord;

ScaleChordAudioProcessor::ScaleChordAudioProcessor()
{
    // default settings
    mapperSettings_.rootNote = 0;
    mapperSettings_.scale = ScaleType::Major;
    voicerSettings_.voicing = VoicingType::Triad;
    voicer_.setSettings(voicerSettings_);
}

ScaleChordAudioProcessor::~ScaleChordAudioProcessor() = default;

void ScaleChordAudioProcessor::prepareToPlay (double /*sampleRate*/, int /*samplesPerBlock*/) {
}

void ScaleChordAudioProcessor::releaseResources() {}

void ScaleChordAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::MidiBuffer output;

    // copy audio through
    for (int ch = 0; ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear (ch, 0, buffer.getNumSamples());

    // Iterate incoming MIDI. For each Note On, map and generate chord note-ons on target channel.
    for (const auto metadata : midiMessages)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn())
        {
            int incomingNote = msg.getNoteNumber();
            int velocity = msg.getVelocity();

            // map note
            int mapped = mapper_.mapNote(incomingNote);

            // make chord
            auto chord = voicer_.makeChordFromNote(mapped);

            // send note-ons for chord on configured channel
            for (int n : chord) {
                juce::MidiMessage m = juce::MidiMessage::noteOn(targetMidiChannel_, n, (juce::uint8)velocity);
                output.addEvent(m, metadata.samplePosition);
            }
        }
        else if (msg.isNoteOff())
        {
            // A robust implementation must track which generated notes belong to which input note
            // and send corresponding note-offs. This example omits book-keeping for brevity.
        }
        else
        {
            // pass-through other messages
            output.addEvent(msg, metadata.samplePosition);
        }
    }

    // replace incoming MIDI with generated output for downstream plugins
    midiMessages.swapWith(output);
}

#endif // JUCE_MODULE_AVAILABLE_juce_audio_processors
